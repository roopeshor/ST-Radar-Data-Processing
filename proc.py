import numpy as np
from scipy.signal.windows import hann


import matplotlib.pyplot as plt
import numpy.typing as npt
from typing import Annotated, Literal


# Define readable aliases for your specific data structures
RadarCube = Annotated[
    npt.NDArray[np.int16], Literal["InCoh", "bins", "nfft"]
]  # Expected shape: (InCoh, RangeBins, NFFT)
from typing import Annotated, Literal

# Expected shape: (RangeBins, NFFT)
PowerSpectrum = Annotated[npt.NDArray[np.float64], Literal["bins", "nfft"]]


def process_radar_data(raw_data_matrix: RadarCube) -> PowerSpectrum:
    # raw_data_matrix shape: (InCoh, RangeBins, NFFT)
    incoh, bins, nfft = raw_data_matrix.shape

    # 1. DC Removal (Subtract the mean along the NFFT time axis)
    dc_removed = raw_data_matrix - np.mean(
        raw_data_matrix, axis=2, keepdims=True
    )

    # 2. Windowing
    window = hann(nfft)
    windowed_data = dc_removed  # * window  # Broadcasting applies it to all integrations/bins

    # 3. FFT (Shift zero-frequency to the center of the array)
    spectra = np.fft.fftshift(np.fft.fft(windowed_data, axis=2), axes=2)

    # Calculate Power Spectrum (Magnitude squared)
    power_spectra = np.abs(spectra) ** 2

    # 4. Incoherent Integration (Average across the InCoh axis)
    averaged_spectra = np.mean(power_spectra, axis=0)

    return averaged_spectra  # Shape: (RangeBins, NFFT)


def visualize_spectra(
    averaged_spectra: PowerSpectrum, range_res_m=150, max_velocity=15.0
):
    """
    Plots the 2D Range-Doppler power spectra.

    averaged_spectra: 2D numpy array of shape (RangeBins, NFFT)
    range_res_m: The physical size of each range bin in meters (e.g., 150m)
    max_velocity: The Nyquist velocity limit of the radar in m/s
    """
    bins: int
    nfft: int
    bins, nfft = averaged_spectra.shape

    # 1. Convert Linear Power to Decibels (dB)
    # Add a tiny epsilon (1e-10) to avoid log10(0) warnings
    power_db = 10 * np.log10(averaged_spectra + 1e-10)

    # 2. Define the Physical Axes Limits for the plot
    # Y-axis: Altitude from 0 to (Total Bins * Resolution)
    max_altitude_km = (bins * range_res_m) / 1000.0

    # X-axis: Velocity from -Vmax to +Vmax
    # (Assuming the FFT was zero-centered using np.fft.fftshift)
    extent = (-max_velocity, max_velocity, 0, max_altitude_km)

    # 3. Create the Plot
    plt.figure(figsize=(10, 8))

    # Use 'turbo' or 'jet' for classic radar visuals, or 'viridis' for modern accessible plots
    im = plt.imshow(
        power_db, aspect="auto", origin="lower", extent=extent, cmap="turbo"
    )

    # Add a colorbar to show the dB scale
    cbar = plt.colorbar(im)
    cbar.set_label("Power (dB)", rotation=270, labelpad=15)

    plt.title("ST Radar Range-Doppler Spectrum")
    plt.xlabel("Doppler Velocity (m/s)")
    plt.ylabel("Altitude (km)")

    plt.grid(color="white", linestyle="--", alpha=0.3)
    plt.tight_layout()
    plt.show()
