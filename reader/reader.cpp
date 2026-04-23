#include "reader.hpp"

#include <fstream>
#include <stdexcept>

vec<BeamObject> Read_data(const std::string& filepath) {
	std::ifstream file(filepath, std::ios::binary);
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open file: " + filepath);
	}

	const int16_t DEFAULT_MAGIC_NUMBER = 369;
	const size_t size_BeamConfig = 732;

	vec<BeamObject> headers;
	int16_t beam_count = 1;	 // Start with 1, update after reading the first header

	for (int beam_No = 0; beam_No < beam_count; ++beam_No) {
		BeamObject beamObj;

		// Read the struct exactly as mapped
		file.read(reinterpret_cast<char*>(&beamObj.cfg), sizeof(RawBeamHeader));

		if (beamObj.cfg.m_sMagicNumber != DEFAULT_MAGIC_NUMBER) {
			throw std::runtime_error("Wrong File Selected: Invalid Magic Number.");
		}

		// MATLAB reads up to 1024 bytes for the full header block. We jump the remainder.
		file.seekg(1024 - size_BeamConfig, std::ios::cur);

		// Update beam_count dynamically from the first valid header read
		beam_count = beamObj.cfg.m_sTotalNumberofBeams;

		int16_t inCohIntegrations = beamObj.cfg.m_sNumOfInCohIntegrations;
		if (inCohIntegrations <= 0) inCohIntegrations = 1;

		int16_t nBins = beamObj.cfg.m_sNumOfRangeBins;
		int16_t nFFT = beamObj.cfg.m_sNFFT;
		size_t dataBlockSize = nBins * nFFT;

		// Allocate 3D Data structure: [InCohIntegration][RangeBin][NFFT]
		beamObj.BeamData.resize(inCohIntegrations, vec<vec<complex>>(nBins, vec<complex>(nFFT)));

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
			vec<complex> rawData(dataBlockSize);
			size_t bytesToRead = dataBlockSize * sizeof(complex);

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
					beamObj.BeamData[InCohInt][r][fft] = rawData[fft * nBins + r];
				}
			}
		}

		headers.push_back(std::move(beamObj));
	}

	return headers;
}

// Helper function to format and print the header
void PrintHeaderSummary(std::ostream& os, const RawBeamHeader& header) {
	// Use std::left and std::setw to align columns neatly
	os << std::left << std::setw(25) << "Magic Number:" << header.m_sMagicNumber << "\n"
	   << std::setw(25) << "Range Bins:" << header.m_sNumOfRangeBins << "\n"
	   << std::setw(25) << "NFFT:" << header.m_sNFFT << "\n"
	   << std::setw(25) << "Coh Integrations:" << header.m_sNumOfCohIntegrations << "\n"
	   << std::setw(25) << "InCoh Integrations:" << header.m_sNumOfInCohIntegrations << "\n"
	   << std::setw(25) << "Total Beams:" << header.m_sTotalNumberofBeams << "\n"
	   << std::setw(25) << "Azimuth:" << header.m_fAzimuth << "\n"
	   << std::setw(25) << "Off-Zenith:" << header.m_fOffZenith << "\n";
}

// Dumps the object to a stream (console or file)
void DumpBeamObject(const BeamObject& beam, const std::string& filename = "") {
	std::ofstream outFile;
	std::ostream* outStream = &std::cout;  // Default to console

	// If a filename is provided, redirect output to the file
	if (!filename.empty()) {
		outFile.open(filename);
		if (!outFile.is_open()) {
			std::cerr << "Failed to open file for writing: " << filename << "\n";
			return;
		}
		outStream = &outFile;
	}

	std::ostream& os = *outStream;

	os << "========================================\n"
	   << "           BEAM CONFIGURATION           \n"
	   << "========================================\n";

	PrintHeaderSummary(os, beam.cfg);

	os << "\n========================================\n"
	   << "             DATA DIMENSIONS            \n"
	   << "========================================\n";

	os << "InCoh Integrations : " << beam.BeamData.size() << "\n";
	if (!beam.BeamData.empty()) {
		os << "Range Bins         : " << beam.BeamData[0].size() << "\n";
		if (!beam.BeamData[0].empty()) {
			os << "NFFT               : " << beam.BeamData[0][0].size() << "\n";
		}
	}

	// Only dump the massive raw data block if we are writing to a file
	if (!filename.empty() && !beam.BeamData.empty()) {
		os << "\n=== Raw Data Dump (Real, Imaginary) ===\n";
		for (size_t intg = 0; intg < beam.BeamData.size(); ++intg) {
			for (size_t r = 0; r < beam.BeamData[intg].size(); ++r) {
				for (size_t f = 0; f < beam.BeamData[intg][r].size(); ++f) {
					const auto& val = beam.BeamData[intg][r][f];
					os << val.real() << "," << val.imag() << "\n";
				}
			}
		}
		std::cout << "Successfully dumped full data to " << filename << "\n";
	}
}

int main() {
	try {
		std::string path =
			"/home/roopesh/Projects/collage/Radar/workshop/Data_Hands_on/Mode_2/"
			"EXP_DBS_CH4_01Jun2025_23_41_50.raw";
		auto beams = Read_data(path);

		for (size_t i = 0; i < beams.size(); ++i) {
			// 1. Print summary to the console cleanly
			DumpBeamObject(beams[i]);

			// 2. Dump summary + full complex data matrix to a CSV-like file
			std::string outFile = "beam_" + std::to_string(i) + "_output.txt";
			DumpBeamObject(beams[i], outFile);
		}
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << "\n";
	}

	return 0;
}