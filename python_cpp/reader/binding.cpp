#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "auto_docs.hpp"
#include "definitions.hpp"
namespace py = pybind11;

PYBIND11_MODULE(st_radar, m) {
	m.doc() = "Python bindings for ST Radar raw data parser";
	py::class_<RawBeamHeader>(m, "RawBeamHeader", "732 byte Beam header")
		// Bind standard scalar fields directly
		// clang-format off
		.def_readonly("rangeBins", &RawBeamHeader::rangeBins, DOC_rangeBins)
		.def_readonly("baudLength_us", &RawBeamHeader::baudLength_us, DOC_baudLength_us)
		.def_readonly("FFTPoints", &RawBeamHeader::FFTPoints, DOC_FFTPoints)
		.def_readonly("numOfCohIntegrations", &RawBeamHeader::numOfCohIntegrations, DOC_numOfCohIntegrations)
		.def_readonly("inCohIntegrations", &RawBeamHeader::inCohIntegrations, DOC_inCohIntegrations)
		.def_readonly("codeFlag", &RawBeamHeader::codeFlag, DOC_codeFlag)
		.def_readonly("IIP_us", &RawBeamHeader::IIP_us, DOC_IIP_us)
		.def_readonly("pulseWidth_us", &RawBeamHeader::pulseWidth_us, DOC_pulseWidth_us)
		.def_readonly("numOfObservedWindows", &RawBeamHeader::numOfObservedWindows, DOC_numOfObservedWindows)
		.def_readonly("stationID", &RawBeamHeader::stationID, DOC_stationID)
		.def_readonly("reserved1", &RawBeamHeader::reserved1, DOC_reserved1)
		.def_readonly("yearCaptured", &RawBeamHeader::yearCaptured, DOC_yearCaptured)
		.def_readonly("yearCaptured", &RawBeamHeader::yearCaptured, DOC_yearCaptured)
		.def_readonly("monthCaptured", &RawBeamHeader::monthCaptured, DOC_monthCaptured)
		.def_readonly("dayCaptured", &RawBeamHeader::dayCaptured, DOC_dayCaptured)
		.def_readonly("hourCaptured", &RawBeamHeader::hourCaptured, DOC_hourCaptured)
		.def_readonly("minCaptured", &RawBeamHeader::minCaptured, DOC_minCaptured)
		.def_readonly("secCaptured", &RawBeamHeader::secCaptured, DOC_secCaptured)
		.def_readonly("reserved4", &RawBeamHeader::reserved4, DOC_reserved4)
		.def_readonly("latitude", &RawBeamHeader::latitude, DOC_latitude)
		.def_readonly("longitude", &RawBeamHeader::longitude, DOC_longitude)
		.def_readonly("altitude", &RawBeamHeader::altitude, DOC_altitude)
		.def_readonly("totalPowerRadiated", &RawBeamHeader::totalPowerRadiated, DOC_totalPowerRadiated)
		.def_readonly("operationMode", &RawBeamHeader::operationMode, DOC_operationMode)
		.def_readonly("currentBeamCount", &RawBeamHeader::currentBeamCount, DOC_currentBeamCount)
		.def_readonly("beamCount", &RawBeamHeader::beamCount, DOC_beamCount)
		.def_readonly("codeLength", &RawBeamHeader::codeLength, DOC_codeLength)
		.def_readonly("TRP_RF_Delay_us", &RawBeamHeader::TRP_RF_Delay_us, DOC_TRP_RF_Delay_us)
		.def_readonly("MGC_Gain_dB", &RawBeamHeader::MGC_Gain_dB, DOC_MGC_Gain_dB)
		.def_readonly("windowType", &RawBeamHeader::windowType, DOC_windowType)
		.def_readonly("reserved5", &RawBeamHeader::reserved5, DOC_reserved5)
		.def_readonly("experimentDataEnabled", &RawBeamHeader::experimentDataEnabled, DOC_experimentDataEnabled)
		.def_readonly("azimuth", &RawBeamHeader::azimuth, DOC_azimuth)
		.def_readonly("offZenith", &RawBeamHeader::offZenith, DOC_offZenith)
		.def_readonly("window1StartHeight", &RawBeamHeader::window1StartHeight,DOC_window1StartHeight)
		.def_readonly("window1EndHeight", &RawBeamHeader::window1EndHeight,DOC_window1EndHeight)
		.def_readonly("window2StartHeight", &RawBeamHeader::window2StartHeight,DOC_window2StartHeight)
		.def_readonly("window2EndHeight", &RawBeamHeader::window2EndHeight,DOC_window2EndHeight)
		.def_readonly("window3StartHeight", &RawBeamHeader::window3StartHeight,DOC_window3StartHeight)
		.def_readonly("window3EndHeight", &RawBeamHeader::window3EndHeight,DOC_window3EndHeight)
		.def_readonly("window4StartHeight", &RawBeamHeader::window4StartHeight,DOC_window4StartHeight)
		.def_readonly("window4EndHeight", &RawBeamHeader::window4EndHeight,DOC_window4EndHeight)
		.def_readonly("window5StartHeight", &RawBeamHeader::window5StartHeight,DOC_window5StartHeight)
		.def_readonly("window5EndHeight", &RawBeamHeader::window5EndHeight,DOC_window5EndHeight)
		// clang-format on
		// Handle C-arrays via lambda properties to prevent breaking the packed memory layout
		.def_property_readonly(
			"clusterWiseRadiatedPower",
			[](const RawBeamHeader& h) {
				return vec<float>(h.clusterWiseRadiatedPower, h.clusterWiseRadiatedPower + 13);
			},
			DOC_clusterWiseRadiatedPower)
		.def_property_readonly(
			"remarks",
			[](const RawBeamHeader& h) { return vec<int8_t>(h.remarks, h.remarks + 512); },
			DOC_remarks)
		.def_property_readonly(
			"reservedCharArr",
			[](const RawBeamHeader& h) {
				return vec<int8_t>(h.reservedCharArr, h.reservedCharArr + 3);
			},
			DOC_reservedCharArr)
		.def_property_readonly(
			"PMCodeA",
			[](const RawBeamHeader& h) { return vec<uint32_t>(h.PMCodeA, h.PMCodeA + 2); },
			DOC_PMCodeA)
		.def_property_readonly(
			"PMCodeB",
			[](const RawBeamHeader& h) { return vec<uint32_t>(h.PMCodeB, h.PMCodeB + 2); },
			DOC_PMCodeB)
		.def_property_readonly(
			"comment",
			[](const RawBeamHeader& h) {
				// Safely convert char array to string, stripping trailing null bytes
				std::string s(reinterpret_cast<const char*>(h.comment), 16);
				s.erase(std::find(s.begin(), s.end(), '\0'), s.end());
				return s;
			},
			DOC_comment);

	py::class_<BeamObject>(m, "BeamObject",
						   "Extended structure to hold both header and the 3D data block")
		.def_readonly("header", &BeamObject::header, "Beam Header data")
		// Automatically converts to nested Python lists
		.def_readonly("BeamData", &BeamObject::BeamData, DOC_BeamData)
		.def_readonly("rangeResolution", &BeamObject::rangeResolution, DOC_rangeResolution)
		.def_readonly("maxVelocity", &BeamObject::maxVelocity, DOC_maxVelocity)
		.def_readonly("direction", &BeamObject::direction, DOC_direction);
		
	m.def("read_data", &Read_data,
		  "Takes .raw radar file path, parses it and returns a list of BeamObjects",
		  py::arg("filepath"));	 // Pull in your structs and function declaration
}