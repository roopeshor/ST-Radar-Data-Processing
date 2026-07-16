#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <numeric>
#include <algorithm>
#include "definitions.hpp"

// Forward declaration of your noise estimator
double estimate_noise_hildebrand_sekhon(const vec<double>& spectrum, int M);

// ---------------------------------------------------------
// Helper: In-place Radix-2 Cooley-Tukey FFT
// ---------------------------------------------------------
void fft(vec<ComplexDouble>& a) {
    int n = a.size();
    if (n <= 1) return;

    // Bit-reversal permutation
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) std::swap(a[i], a[j]);
    }

    // Cooley-Tukey
    for (int len = 2; len <= n; len <<= 1) {
        double angle = -2 * M_PI / len;
        ComplexDouble wlen(std::cos(angle), std::sin(angle));
        for (int i = 0; i < n; i += len) {
            ComplexDouble w(1);
            for (int j = 0; j < len / 2; j++) {
                ComplexDouble u = a[i + j];
                ComplexDouble v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }
}

// Helper: FFT Shift (Swaps left and right halves)
void fftshift(vec<ComplexDouble>& a) {
    std::rotate(a.begin(), a.begin() + a.size() / 2, a.end());
}

/**
 * Processese a single power spectrum of size inCoh x heightBins x nFFT points
 */
PowerSpectrum process_radar_data(const RadarCube& raw_data_matrix) {
    if (raw_data_matrix.empty() || raw_data_matrix[0].empty() || raw_data_matrix[0][0].empty()) {
        return {};
    }

    int incoh = raw_data_matrix.size();
    int bins = raw_data_matrix[0].size();
    int nfft = raw_data_matrix[0][0].size();

    PowerSpectrum averaged_spectra(bins, vec<double>(nfft, 0.0));
    double global_max_power = 0.0;

    // We process bin by bin, accumulating over the InCoh axis
    for (int i = 0; i < incoh; ++i) {
        for (int b = 0; b < bins; ++b) {
            // Copy the time series for this specific integration and bin
            vec<ComplexDouble> time_series = raw_data_matrix[i][b];

            // 1. DC Removal
            ComplexDouble sum = 0;
            for (int f = 0; f < nfft; ++f) sum += time_series[f];
            ComplexDouble mean = sum / static_cast<double>(nfft);
            
            for (int f = 0; f < nfft; ++f) {
                time_series[f] -= mean;
            }

            // 2. Windowing (Bypassed, just like the Python script)

            // 3. FFT & Shift
            fft(time_series);
            fftshift(time_series);

            // 4. Power calculation (Magnitude squared) and accumulation
            for (int f = 0; f < nfft; ++f) {
                double power = std::norm(time_series[f]); // std::norm is |x|^2 in C++
                averaged_spectra[b][f] += power;
                
                if (power > global_max_power) {
                    global_max_power = power; // Replicating np.max(power_spectra)
                }
            }
        }
    }

    // 5. Divide by InCoh (Mean) and Global Max (Normalization)
    if (global_max_power > 0) {
        for (int b = 0; b < bins; ++b) {
            for (int f = 0; f < nfft; ++f) {
                averaged_spectra[b][f] = (averaged_spectra[b][f] / incoh) / global_max_power;
            }
        }
    }

    // 6. Noise Estimation & Subtraction
    for (int b = 0; b < bins; ++b) {
        // Calculate noise floor for this specific altitude bin
        double L = estimate_noise_hildebrand_sekhon(averaged_spectra[b], incoh);
        
        // Subtract the noise floor from all frequencies in this bin (broadcasting equivalent)
        for (int f = 0; f < nfft; ++f) {
            averaged_spectra[b][f] -= L;
            
            // Standard DSP Practice: Clamp to 0 to prevent negative power 
            // (Uncomment if needed for your moment extraction)
            if (averaged_spectra[b][f] < 0) averaged_spectra[b][f] = 0;
        }
    }

    return averaged_spectra;
}