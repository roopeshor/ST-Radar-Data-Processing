function [averaged_power_spectra, noise_level] = process_radar_data(raw_data_matrix)
    % process_radar_data Process the radar raw data to generate power spectra and noise level.
    % raw_data_matrix shape from Read_data: (RangeBins, NFFT, InCohIntegrations)
    
    [bins, nfft, incoh] = size(raw_data_matrix);
    
    % 1. DC Removal (Subtract the mean along the NFFT time axis - which is dim 2)
    dc_mean = mean(raw_data_matrix, 2);
    dc_removed = raw_data_matrix - dc_mean;
    
    % 2. Windowing with hann
    n = 0:(nfft-1);
    window = 0.5 * (1 - cos(2 * pi * n / (nfft - 1)));
    
    % Reshape window to match (1, NFFT, 1) for broadcasting
    % ie, repeats it along height direction
    window_reshaped = reshape(window, [1, nfft, 1]);
    windowed_data = dc_removed .* window_reshaped;
    
    % 3. FFT (Shift zero-frequency to the center of the array)
    % FFT along dimension 2
    spectra = fft(windowed_data, [], 2);
    spectra = fftshift(spectra, 2);
    
    % Calculate Power Spectrum (Magnitude squared and normalized)
    power_spectra = abs(spectra).^2;
    power_spectra = power_spectra / max(power_spectra(:));
    
    % 4. Incoherent Integration (Average across the InCoh axis - dim 3)
    averaged_power_spectra = mean(power_spectra, 3);
    
    % Squeeze to remove InCoh axis
    averaged_power_spectra = squeeze(averaged_power_spectra);
    
    averaged_power_spectra = averaged_power_spectra / max(averaged_power_spectra(:));
    
    % 5. Noise estimation
    noise_level = estimate_noise_hildebrand_sekhon(averaged_power_spectra, incoh);
end
