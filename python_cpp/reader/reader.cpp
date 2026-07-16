#include "definitions.hpp"

#include <fstream>
#include <stdexcept>

/**
 * Reads given file path and returns BeamObject for it
 */
vec<BeamObject> Read_data(const std::string& filepath) {
	std::ifstream file(filepath, std::ios::binary);
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open file: " + filepath);
	}

	vec<BeamObject> headers;
	int16_t beam_count = 1;	 // Start with 1, update after reading the first header
	for (int beam_No = 0; beam_No < beam_count; ++beam_No) {
		BeamObject beamObj;

		// Read the struct exactly as mapped
		file.read(reinterpret_cast<char*>(&beamObj.header), sizeof(RawBeamHeader));

		if (beamObj.header.fileMagicNumber != DEFAULT_MAGIC_NUMBER) {
			throw std::runtime_error("Wrong File Selected: Invalid Magic Number.");
		}

		// MATLAB reads up to 1024 bytes for the full header block. We jump the remainder.
		file.seekg(1024 - BEAM_HEADER_SIZE, std::ios::cur);

		// Update beam_count dynamically from the first valid header read
		beam_count = beamObj.header.beamCount;

		int16_t inCohIntegrations = beamObj.header.inCohIntegrations;
		if (inCohIntegrations <= 0) inCohIntegrations = 1;

		int16_t nBins = beamObj.header.rangeBins;
		int16_t nFFT = beamObj.header.FFTPoints;
		size_t dataBlockSize = nBins * nFFT;

		// Allocate 3D Data structure: [InCohIntegration][RangeBin][NFFT]
		beamObj.BeamData.resize(inCohIntegrations,
								vec<vec<complex32>>(nBins, vec<complex32>(nFFT)));

		for (int InCohInt = 0; InCohInt < inCohIntegrations; ++InCohInt) {
			if (InCohInt != 0) {
				// Subsequent integrations are prefaced with a dummy/secondary 1024-byte header
				vec<int8_t> dummyHeader(1024);
				file.read(reinterpret_cast<char*>(dummyHeader.data()), 1024);
				if (file.gcount() != 1024) {
					throw std::runtime_error("Error in file Read: Expected Header Data Count 1024");
				}
			}

			// Read the block of complex integers
			// C++ std::complex<int32_t> is layout-compatible with int32_t[2]
			vec<complex32> rawData(dataBlockSize);
			size_t bytesToRead = dataBlockSize * sizeof(complex32);

			size_t currentPos = file.tellg();
			file.read(reinterpret_cast<char*>(rawData.data()), bytesToRead);
			size_t bytesRead = static_cast<size_t>(file.tellg()) - currentPos;

			if (bytesRead != bytesToRead) {
				throw std::runtime_error("Error in file Read: Expected Data Count mismatch.");
			}

			// Map 1D flat interleaved array into 2D (Range x FFT) matrix
			// Equivalent to MATLAB's transpose and reshape operations
			for (int fft = 0; fft < nFFT; ++fft) {
				for (int r = 0; r < nBins; ++r) {
					// MATLAB logic implies sequential elements belong to range bins per
					// FFT loop
					beamObj.BeamData[InCohInt][r][fft] = std::conj(rawData[fft * nBins + r]);
				}
			}
		}

		beamObj.rangeResolution = getRangeResolution(beamObj.header);
		beamObj.maxVelocity = getRangeResolution(beamObj.header);
		beamObj.direction = getBeamDirection(beamObj.header.azimuth, beamObj.header.offZenith);
		headers.push_back(std::move(beamObj));
	}

	return headers;
}

float getRangeResolution(RawBeamHeader beamHeader) {
	float c = 299792458.0;
	// Check if phase-coding (pulse compression) is enabled
	// Usually, 1 or >0 means enabled, 0 means disabled
	float effective_time_us;
	if (beamHeader.codeFlag > 0) {
		// Use Baud Length (the width of the compressed "chip")
		effective_time_us = beamHeader.baudLength_us;
	} else {
		// Use the full uncompressed Pulse Width
		effective_time_us = beamHeader.pulseWidth_us;
	}

	// Calculate Delta R
	return (c * effective_time_us * 1e-6) / 2.0;
}

/**
 * Calculates the Nyquist velocity (maximum unambiguous velocity) in m/s. Defaults to 53 MHz
 * (Standard VHF ST Radar frequency).
 */
float getMaxVelocity(RawBeamHeader beamHeader, float radarFreq = 205e6) {
	float c = 299792458.0;
	float wavelength = c / radarFreq;
	// 2. Calculate the effective time between FFT samples(in seconds)
	float effective_sampling_time = beamHeader.IIP_us * 1e-6 * beamHeader.numOfCohIntegrations;
	// 3. Calculate Vmax
	return wavelength / (4.0 * effective_sampling_time);
}

std::string getBeamDirection(int azimuth, float offZenith) {
	if (offZenith == 0) return "Vertical";
	switch (azimuth) {
		case 0: return "North";
		case 90: return "West";
		case 180: return "South";
		case 270: return "East";
	}
	return "Unknown";
}