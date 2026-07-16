module Proc

using FFTW
using Statistics
using Plots
include("Reader.jl")
using .Reader

export process_radar_data, estimate_noise_hildebrand_sekhon, visualize_spectra, compute_woodman_moments

const RadarCube = Array{ComplexF32, 3}
"""
Raw radar data
size: 1x173x1024 (InCoh, RangeBins, NFFT)
"""

const PowerSpectrum = Array{Float64, 2}
"""
Power spectrum
size: 173x1024 (RangeBins, NFFT)
"""

const NoiseLevel = Array{Float64, 1}
"""
computed noise leven in each height bins. 
size: 173
"""

function process_radar_data(raw_data_matrix::RadarCube)::Tuple{PowerSpectrum, NoiseLevel}
    incoh, bins, nfft = size(raw_data_matrix)

    # 1. DC Removal (Subtract the mean along the NFFT time axis)
    # mean over axis 3 (NFFT)
    dc_mean = mean(raw_data_matrix, dims=3)
    dc_removed = raw_data_matrix .- dc_mean

    # 2. Windowing
    # Exact replica of scipy.signal.windows.hann(nfft, sym=True)
    window = 0.5 .* (1 .- cos.(2 * pi .* (0:nfft-1) ./ (nfft - 1)))
    
    # Reshape for broadcasting along axis 3
    window_reshaped = reshape(window, 1, 1, nfft)
    windowed_data = dc_removed .* window_reshaped

    # 3. FFT (Shift zero-frequency to the center of the array)
    # FFT over axis 3, then fftshift over axis 3
    spectra = fftshift(fft(windowed_data, 3), 3)

    # Calculate Power Spectrum (Magnitude squared)
    power_spectra = abs.(spectra) .^ 2
    max_power = maximum(power_spectra)
    power_spectra = power_spectra ./ max_power

    # 4. Incoherent Integration (Average across the InCoh axis)
    # Mean over axis 1
    averaged_spectras = dropdims(mean(power_spectra, dims=1), dims=1)
    
    max_avg_power = maximum(averaged_spectras)
    averaged_spectras = averaged_spectras ./ max_avg_power

    L = estimate_noise_hildebrand_sekhon(averaged_spectras, incoh)

    return (averaged_spectras, L)
end

function estimate_noise_hildebrand_sekhon(spectrums::PowerSpectrum, M::Int)::NoiseLevel
    """
    Estimates the noise level threshold (L) of a power spectrum.

    Args:
        spectrum: 2D array representing the power spectrum for a single range bin.
        M: Number of incoherent integrations (numOfInCohIntegrations).

    Returns:
        The estimated noise level threshold (L).
    """
    # Step 1: Reorder the spectrum in ascending order (A_i)
    # In Python: np.sort(spectrums, axis=-1)
    A = mapslices(sort, spectrums, dims=2)
    bins, N = size(spectrums)

    # Create an array of divisors: [1, 2, 3, ..., N]
    n_arr = 1:N
    nm = repeat(reshape(n_arr, 1, N), bins, 1)

    # Step 2: Compute P_n (running mean) and Q_n (running variance)
    # P_n = cumsum(A, axis=1) / nm
    P_n = cumsum(A, dims=2) ./ nm

    # Q_n = (cumsum(A**2, axis=1) / n_arr) - (P_n**2)
    Q_n = (cumsum(A .^ 2, dims=2) ./ reshape(n_arr, 1, N)) .- (P_n .^ 2)

    # Compute R_n
    # R_n = (P_n**2) / (Q_n * M)
    R_n = (P_n .^ 2) ./ (Q_n .* M)
    # Replace NaNs/Infs with 0 or similar to python ignore
    # Actually, in python `> 1.0` on NaN evaluates to False. So no need to strictly filter NaN.

    # Step 3: Find the cutoff index k
    noise_level = zeros(Float64, bins)
    for i in 1:bins
        arr = reverse(R_n[i, :])
        
        idx_0based = 0
        for j in 1:length(arr)
            # handle NaN > 1.0 which is false
            if !isnan(arr[j]) && arr[j] > 1.0
                idx_0based = j - 1
                break
            end
        end
        
        valid_idx_0based = N - 1 - idx_0based
        noise_level[i] = P_n[i, valid_idx_0based + 1] # +1 for 1-based index
    end

    return noise_level
end

function plot_subgrid(power_db, extent, title)
    # power_db is (RangeBins, NFFT)
    # In Plots.jl, heatmap expects x, y, z where z is transpose of what matplotlib takes 
    # if we just pass the matrix, it uses row as y, col as x?
    # Actually heatmap(x, y, matrix')
    x = range(extent[1], stop=extent[2], length=size(power_db, 2))
    y = range(extent[3], stop=extent[4], length=size(power_db, 1))
    
    return heatmap(x, y, power_db, 
                   title=title, 
                   xlabel="Doppler Velocity (m/s)", 
                   ylabel="Altitude (km)",
                   color=:turbo,
                   legend=false)
end

function visualize_spectra(beams::Vector{BeamObject})
    """
    Plots the tile of 2D Range-Doppler power spectra in 5 directions.
    """
    plots_arr = []
    
    for (i, beam) in enumerate(beams)
        beam_spectra, L = process_radar_data(beam.BeamData)
        max_velocity = beam.maxVelocity
        start_height = beam.header.window1StartHeight
        bins, nfft = size(beam_spectra)
        
        noiseLevelPerBin = repeat(L, 1, nfft)
        beam_spectra .-= noiseLevelPerBin
        
        # log10 safely
        power_db = zeros(Float64, size(beam_spectra))
        for idx in eachindex(beam_spectra)
            if beam_spectra[idx] > 0
                power_db[idx] = 10 * log10(beam_spectra[idx])
            else
                power_db[idx] = -Inf # or NaN
            end
        end
        
        max_altitude = beam.header.window1EndHeight
        
        extent = (-max_velocity, max_velocity, start_height, max_altitude)
        p = plot_subgrid(power_db, extent, beam.direction)
        push!(plots_arr, p)
    end
    
    # Display in a grid
    plt = plot(plots_arr..., layout=(2, 3), size=(1000, 800))
    display(plt)
end

function compute_woodman_moments(spectrum_shifted::PowerSpectrum, L::NoiseLevel, ipp_us::Float32, n_coh::Int)::Tuple{Float64, Float64}
    bins, nfft = size(spectrum_shifted)
    
    noise_level_L = repeat(L, 1, nfft)
    P_tilde = spectrum_shifted .- noise_level_L
    P_tilde[P_tilde .< 0] .= 0.0
    
    # Note: Julia uses 1-based indexing
    # We will replicate the python logic exactly using 1-based indexing.
    # We only operate on 1D spectrum here? The python signature expects 2D PowerSpectrum but then computes l = np.argmax(P_tilde) on the FLATTENED array?
    # Wait, in python: `l = np.argmax(P_tilde)` returns the flattened index!
    # And then `m = l` and iterates. This means Python code assumes `P_tilde` is effectively 1D or treats it as flattened array of all bins combined!
    # Let's verify python code:
    # l = np.argmax(P_tilde)
    # P_tilde[m-1], P_tilde[n+1]
    # Yes, it treats it as 1D array of size bins*nfft, or `bins` is 1?
    # If `compute_woodman_moments` is called with a single range bin, it works.
    
    # We will implement exactly as python:
    flattened_P = vec(P_tilde) # flat view
    l = argmax(flattened_P)
    
    if flattened_P[l] == 0
        return (0.0, 0.0)
    end
    
    m = l
    while m > 1 && flattened_P[m - 1] > 0
        m -= 1
    end
    
    n = l
    while n < length(flattened_P) && flattened_P[n + 1] > 0
        n += 1
    end
    
    signal_block = flattened_P[m:n]
    indices = m:n
    
    M0 = sum(signal_block)
    
    # Python: indices = np.arange(m, n+1), 0-based.
    # Julia indices: m:n (1-based). To match Python indices mathematically:
    py_indices = indices .- 1
    
    T_eff = (ipp_us * 1e-6) * n_coh
    f_i = (py_indices .- (bins / 2)) ./ (T_eff * bins) # Wait, python uses `bins` here which is the first dimension size!
    # Actually if python receives 1D array of shape (N,), bins = N.
    # Wait, python code: `(bins, nfft) = spectrum_shifted.shape`. If it's a 2D array, python argmax is global.
    # But `bins / 2` and `T_eff * bins` in Python implies `bins` was supposed to be the NFFT length!
    # Let's keep Python's exact math: `bins` is the row count.
    
    M1 = sum(signal_block .* f_i) / M0
    
    return (M0, M1)
end

end # module
