function [M0, M1] = compute_woodman_moments(spectrum_shifted, L, ipp_us, n_coh)
    % compute_woodman_moments Computes Woodman moments M0 and M1.
    
    [bins, nfft] = size(spectrum_shifted);
    
    noise_level_L = repmat(L(:), 1, nfft);
    
    P_tilde = spectrum_shifted - noise_level_L;
    P_tilde(P_tilde < 0) = 0.0;
    
    % Emulate Python's flat index (row-major)
    P_flat_py = reshape(P_tilde.', [], 1);
    
    [~, l] = max(P_flat_py);
    
    if P_flat_py(l) == 0
        M0 = 0.0;
        M1 = 0.0;
        return;
    end
    
    m = l;
    while m > 1 && P_flat_py(m - 1) > 0
        m = m - 1;
    end
    
    n = l;
    while n < length(P_flat_py) && P_flat_py(n + 1) > 0
        n = n + 1;
    end
    
    signal_block = P_flat_py(m:n);
    indices = m:n;
    
    M0 = sum(signal_block);
    
    % Map back to Python's 0-based index for math
    py_indices = indices - 1;
    
    T_eff = (ipp_us * 1e-6) * n_coh;
    % Note: Python uses bins here. If called on 1D array, bins == length.
    f_i = (py_indices - (bins / 2)) / (T_eff * bins);
    
    M1 = sum(signal_block .* f_i(:)) / M0;
end
