#include "definitions.hpp"
#include <fstream>

// Helper function to format and print the header
void PrintHeaderSummary(std::ostream& os, const RawBeamHeader& header) {
	// Use std::left and std::setw to align columns neatly
	os << std::left << std::setw(25) << "Magic Number:" << header.fileMagicNumber << "\n"
	   << std::setw(25) << "Range Bins:" << header.rangeBins << "\n"
	   << std::setw(25) << "NFFT:" << header.FFTPoints << "\n"
	   << std::setw(25) << "Coh Integrations:" << header.numOfCohIntegrations << "\n"
	   << std::setw(25) << "InCoh Integrations:" << header.inCohIntegrations << "\n"
	   << std::setw(25) << "Total Beams:" << header.beamCount << "\n"
	   << std::setw(25) << "Azimuth:" << header.azimuth << "\n"
	   << std::setw(25) << "Off-Zenith:" << header.offZenith << "\n";
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

	PrintHeaderSummary(os, beam.header);

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
