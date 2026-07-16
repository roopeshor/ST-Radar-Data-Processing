function visualize_spectra(beams)
    % visualize_spectra Plots the tile of 2D Range-Doppler power spectra.
    
    figure('Position', [100, 100, 1000, 800]);
    
    for i = 1:length(beams)
        beam = beams(i);
        
        [beam_spectra, L] = process_radar_data(beam.BeamData);
        
        % Calculate max_velocity (Nyquist)
        radarFreq = 205e6;
        c = 299792458.0;
        wavelength = c / radarFreq;
        effective_sampling_time = beam.m_fIntrPulsePeriod_us * 1e-6 * beam.m_sNumOfCohIntegrations;
        max_velocity = wavelength / (4.0 * effective_sampling_time);
        
        start_height = beam.m_fWindow1StartHeight;
        max_altitude = beam.m_fWindow1EndHeight;
        
        [bins, nfft] = size(beam_spectra);
        
        noiseLevelPerBin = repmat(L, 1, nfft);
        beam_spectra = beam_spectra - noiseLevelPerBin;
        
        % Prevent negative values which result in complex logarithms
        beam_spectra(beam_spectra < 0) = 0;
        
        % Calculate Power dB
        power_db = zeros(size(beam_spectra));
        
        warning('off', 'MATLAB:log:logOfZero');
        power_db = 10 * log10(beam_spectra);
        warning('on', 'MATLAB:log:logOfZero');
        
        % Direction
        direction = getBeamDirection(beam.m_fAzimuth, beam.m_fOffZenith);
        
        extent = [-max_velocity, max_velocity, start_height, max_altitude];
        
        subplot(2, 3, i);
        
        % imagesc uses [xmin xmax], [ymin ymax]
        % In MATLAB, imagesc puts y-axis top-down by default. 'YDir', 'normal' flips it.
        imagesc(extent(1:2), extent(3:4), power_db);
        set(gca, 'YDir', 'normal');
        colormap('turbo');
        
        title(direction);
        xlabel('Doppler Velocity (m/s)');
        ylabel('Altitude (km)');
        
        grid on;
        set(gca, 'GridColor', 'k', 'GridLineStyle', '--', 'GridAlpha', 0.3);
    end
end

function dir_str = getBeamDirection(azimuth, offZenith)
    if offZenith == 0
        dir_str = 'Vertical';
        return;
    end
    azimuth_int = round(azimuth);
    switch azimuth_int
        case 0
            dir_str = 'North';
        case 90
            dir_str = 'West';
        case 180
            dir_str = 'South';
        case 270
            dir_str = 'East';
        otherwise
            dir_str = 'Unknown';
    end
end
