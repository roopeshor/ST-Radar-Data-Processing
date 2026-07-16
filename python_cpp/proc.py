from reader.st_radar import BeamObject
import numpy as np

import matplotlib.pyplot as plt
import numpy.typing as npt
from scipy.signal.windows import hann

RadarCube = npt.NDArray[np.int16]
"""
Raw radar data
size: 1x173x1024 (InCoh, RangeBins, NFFT)
"""


PowerSpectrum = npt.NDArray[np.float64]
"""
Power spectrum
size: 173x1024 (RangeBins, NFFT)
"""

NoiseLevel = npt.NDArray[np.float64]
"""
computed noise leven in each height bins. 
size: 173
"""


def process_radar_data(
    raw_data_matrix: RadarCube,
) -> tuple[PowerSpectrum, NoiseLevel]:
    incoh, bins, nfft = raw_data_matrix.shape

    # 1. DC Removal (Subtract the mean along the NFFT time axis)

    dc_removed: PowerSpectrum = raw_data_matrix - np.mean(
        raw_data_matrix, axis=2, keepdims=True
    )

    # 2. Windowing
    window = hann(nfft)
    windowed_data = (
        dc_removed * window
    )  # Broadcasting applies it to all integrations/bins

    # 3. FFT (Shift zero-frequency to the center of the array)
    spectra = np.fft.fftshift(np.fft.fft(windowed_data, axis=2), axes=2)

    # Calculate Power Spectrum (Magnitude squared)
    power_spectra = np.abs(spectra) ** 2
    power_spectra = power_spectra / np.max(power_spectra)

    # 4. Incoherent Integration (Average across the InCoh axis)
    averaged_spectras = np.mean(power_spectra, axis=0) / np.max(power_spectra)

    L = estimate_noise_hildebrand_sekhon(averaged_spectras, incoh)

    return (averaged_spectras, L)  # Shape: (RangeBins, NFFT)


def estimate_noise_hildebrand_sekhon(
    spectrums: list[PowerSpectrum], M: int
) -> NoiseLevel:
    """
    Estimates the noise level threshold (L) of a power spectrum.

    Args:
        spectrum: 1D numpy array representing the power spectrum for a single range bin.
        M: Number of incoherent integrations (numOfInCohIntegrations).

    Returns:
        The estimated noise level threshold (L).
    """
    # Step 1: Reorder the spectrum in ascending order (A_i)
    A = np.sort(spectrums)
    bins = len(spectrums)
    N = len(spectrums[0])

    # Create an array of divisors: [1, 2, 3, ..., N]
    n_arr = np.arange(1, N + 1)
    nm = np.zeros((bins, N), dtype=float)

    # nm = [
    #     [1, 2, 3, ... N],
    #     [1, 2, 3, ... N],
    #     ...
    #     [1, 2, 3, ... N],
    # ]
    nm[:, :] = n_arr[np.newaxis :]

    # Step 2: Compute P_n (running mean) and Q_n (running variance)
    # P_n = [
    #   [ 1, 2, 6, .. ]
    #   [ 1, 2, 6, .. ]
    #   ...
    # ]
    # size: <173x1024>
    P_n = np.cumsum(A, axis=1) / nm

    # size: <173x1024>
    Q_n = (np.cumsum(A**2, axis=1) / n_arr) - (P_n**2)

    # Compute R_n
    with np.errstate(divide="ignore", invalid="ignore"):
        # size: <173x1024>
        R_n = (P_n**2) / (Q_n * M)

    # Step 3: Find the cutoff index k

    valid_indices = N - 1 - np.argmax(R_n[:, ::-1] > 1.0, axis=1)
    if len(valid_indices) > 0:
        noise_level = P_n[np.arange(bins), valid_indices]
    else:
        noise_level = P_n[np.arange(bins), np.argmax(nm, axis=1)]
    return noise_level


def visualize_spectra(beams: list[BeamObject]):
    """
    Plots the tile of 2D Range-Doppler power spectra in 5 directions.

    averaged_spectra: 2D numpy array of shape (RangeBins, NFFT)
    range_res_m: The physical size of each range bin in meters (e.g., 150m).
    The plot plots freq spectrum in all directions.
    max_velocity: The Nyquist velocity limit of the radar in m/s
    """

    plt.figure(figsize=(10, 8))
    for i, beam in enumerate(beams):
        (beam_spectra, L) = process_radar_data(np.array(beam.BeamData))
        max_velocity = beam.maxVelocity
        start_height = beam.header.window1StartHeight
        (bins, nfft) = beam_spectra.shape

        noiseLevelPerBin = np.zeros((bins, nfft), dtype=float)
        noiseLevelPerBin[:, :] = L[:, np.newaxis]
        beam_spectra -= noiseLevelPerBin

        power_db: np.ndarray
        with np.errstate(divide="ignore", invalid="ignore"):
            power_db = 10 * np.log10(beam_spectra)

        max_altitude: float = beam.header.window1EndHeight

        # range_res_m = beam.rangeResolution
        # heightBins, _ = beam_spectra.shape
        # (
        #     end_height
        #     if end_height > 0
        #     else heightBins * range_res_m + start_height
        # )

        # X-axis: Velocity from -Vmax to +Vmax
        # (Assuming the FFT was zero-centered using np.fft.fftshift)
        extent = (-max_velocity, max_velocity, start_height, max_altitude)
        plot_subgrid(power_db, i + 1, extent, beam.direction)
    plt.tight_layout()
    plt.show()


def plot_subgrid(power_db, i, extent, title):
    plt.subplot(2, 3, i)

    # Use 'turbo' or 'jet' for classic radar visuals, or 'viridis' for modern accessible plots
    im = plt.imshow(
        power_db, aspect="auto", origin="lower", extent=extent, cmap="turbo"
    )

    # Add a colorbar to show the dB scale
    # cbar = plt.colorbar(im)
    # cbar.set_label("Power (dB)", rotation=270, labelpad=15)

    plt.title(title)
    plt.xlabel("Doppler Velocity (m/s)")
    plt.ylabel("Altitude (km)")

    plt.grid(color="black", linestyle="--", alpha=0.3)


def compute_woodman_moments(
    spectrum_shifted: PowerSpectrum, L: NoiseLevel, ipp_us, n_coh
) -> tuple[float,float]:

    (bins, nfft) = spectrum_shifted.shape
    noise_level_L = np.zeros((bins, nfft), dtype=np.float64)

    # Fill the array by broadcasting the column to all rows
    noise_level_L[:, :] = L[:, np.newaxis]
    # Step 2: Subtract noise and clip at 0
    P_tilde = spectrum_shifted - noise_level_L
    P_tilde[P_tilde < 0] = 0.0

    # Step 3i: Find the peak index 'l'
    l = np.argmax(P_tilde)

    # If the peak is exactly 0, there is no signal, only noise
    if P_tilde[l] == 0:
        return 0.0, 0.0  # M0, M1

    # Step 3ii: Find lower bound 'm'
    m = l
    while m > 0 and P_tilde[m - 1] > 0:
        m -= 1

    # Step 3iii: Find upper bound 'n'
    n = l
    while n < bins - 1 and P_tilde[n + 1] > 0:
        n += 1

    # Isolate the signal and indices
    signal_block = P_tilde[m : n + 1]
    indices = np.arange(m, n + 1)

    # Step 4i: Compute M0 (Total Power)
    M0 = np.sum(signal_block)

    # Calculate physical frequencies for the isolated block
    # Using the corrected equation (14)
    T_eff = (ipp_us * 1e-6) * n_coh
    f_i = (indices - (bins / 2)) / (T_eff * bins)

    # Step 4ii: Compute M1 (Mean Doppler Frequency in Hz)
    M1 = np.sum(signal_block * f_i) / M0

    return M0, M1

# To convert M1 (Hz) to physical wind velocity (m/s):
# Velocity = -1.0 * M1 * (Wavelength / 2.0)

