#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "auto_docs.hpp"
#include "reader.hpp"
namespace py = pybind11;

PYBIND11_MODULE(st_radar, m) {
	m.doc() = "Python bindings for ST Radar raw data parser";

	py::class_<RawBeamHeader>(m, "RawBeamHeader")
		// Bind standard scalar fields directly
		.def_readonly("m_sNumOfRangeBins", &RawBeamHeader::m_sNumOfRangeBins, DOC_m_sNumOfRangeBins)
		.def_readonly("m_fBaudLength_us", &RawBeamHeader::m_fBaudLength_us, DOC_m_fBaudLength_us)
		.def_readonly("m_sNFFT", &RawBeamHeader::m_sNFFT, DOC_m_sNFFT)
		.def_readonly("m_sNumOfCohIntegrations", &RawBeamHeader::m_sNumOfCohIntegrations,
					  DOC_m_sNumOfCohIntegrations)
		.def_readonly("m_sNumOfInCohIntegrations", &RawBeamHeader::m_sNumOfInCohIntegrations,
					  DOC_m_sNumOfInCohIntegrations)
		.def_readonly("m_sCodeFlag", &RawBeamHeader::m_sCodeFlag, DOC_m_sCodeFlag)
		.def_readonly("m_fIntrPulsePeriod_us", &RawBeamHeader::m_fIntrPulsePeriod_us,
					  DOC_m_fIntrPulsePeriod_us)
		.def_readonly("m_fPulseWidth_us", &RawBeamHeader::m_fPulseWidth_us, DOC_m_fPulseWidth_us)
		.def_readonly("m_sNumOfObservWindows", &RawBeamHeader::m_sNumOfObservWindows,
					  DOC_m_sNumOfObservWindows)
		.def_readonly("m_sStationID", &RawBeamHeader::m_sStationID, DOC_m_sStationID)
		.def_readonly("m_sReserved1", &RawBeamHeader::m_sReserved1, DOC_m_sReserved1)
		.def_readonly("m_sYear", &RawBeamHeader::m_sYear, DOC_m_sYear)
		.def_readonly("m_sReserved4", &RawBeamHeader::m_sReserved4, DOC_m_sReserved4)
		.def_readonly("m_fLatitude", &RawBeamHeader::m_fLatitude, DOC_m_fLatitude)
		.def_readonly("m_fLongitude", &RawBeamHeader::m_fLongitude, DOC_m_fLongitude)
		.def_readonly("m_fAltitude", &RawBeamHeader::m_fAltitude, DOC_m_fAltitude)
		.def_readonly("m_fTotalPowerRadiated", &RawBeamHeader::m_fTotalPowerRadiated,
					  DOC_m_fTotalPowerRadiated)
		.def_readonly("m_sOperationMode", &RawBeamHeader::m_sOperationMode, DOC_m_sOperationMode)
		.def_readonly("m_sCurrentBeamCnt", &RawBeamHeader::m_sCurrentBeamCnt, DOC_m_sCurrentBeamCnt)
		.def_readonly("m_sTotalNumberofBeams", &RawBeamHeader::m_sTotalNumberofBeams,
					  DOC_m_sTotalNumberofBeams)
		.def_readonly("m_usCodeLength", &RawBeamHeader::m_usCodeLength, DOC_m_usCodeLength)
		.def_readonly("m_fTRP_RF_Delay_us", &RawBeamHeader::m_fTRP_RF_Delay_us,
					  DOC_m_fTRP_RF_Delay_us)
		.def_readonly("m_fMGC_Gain_dB", &RawBeamHeader::m_fMGC_Gain_dB, DOC_m_fMGC_Gain_dB)
		.def_readonly("m_usWindowType", &RawBeamHeader::m_usWindowType, DOC_m_usWindowType)
		.def_readonly("m_usReserved", &RawBeamHeader::m_usReserved, DOC_m_usReserved)
		.def_readonly("m_ucExperimentDataEnable", &RawBeamHeader::m_ucExperimentDataEnable,
					  DOC_m_ucExperimentDataEnable)
		.def_readonly("m_fAzimuth", &RawBeamHeader::m_fAzimuth, DOC_m_fAzimuth)
		.def_readonly("m_fOffZenith", &RawBeamHeader::m_fOffZenith, DOC_m_fOffZenith)
		.def_readonly("m_fWindow1StartHeight", &RawBeamHeader::m_fWindow1StartHeight,
					  DOC_m_fWindow1StartHeight)

		// Handle C-arrays via lambda properties to prevent breaking the packed memory layout
		.def_property_readonly(
			"m_fRadiatedPower_ClusterWise",
			[](const RawBeamHeader& h) {
				return vec<float>(h.m_fRadiatedPower_ClusterWise,
								  h.m_fRadiatedPower_ClusterWise + 13);
			},
			DOC_m_fRadiatedPower_ClusterWise)
		.def_property_readonly(
			"m_cArrRemarks",
			[](const RawBeamHeader& h) {
				return vec<int8_t>(h.m_cArrRemarks, h.m_cArrRemarks + 512);
			},
			DOC_m_cArrRemarks)
		.def_property_readonly(
			"m_cReserved",
			[](const RawBeamHeader& h) { return vec<int8_t>(h.m_cReserved, h.m_cReserved + 3); },
			DOC_m_cReserved)
		.def_property_readonly(
			"m_ulCodeA",
			[](const RawBeamHeader& h) { return vec<uint32_t>(h.m_ulCodeA, h.m_ulCodeA + 2); },
			DOC_m_ulCodeA)
		.def_property_readonly(
			"m_ulCodeB",
			[](const RawBeamHeader& h) { return vec<uint32_t>(h.m_ulCodeB, h.m_ulCodeB + 2); },
			DOC_m_ulCodeB)
		.def_property_readonly(
			"m_cComment",
			[](const RawBeamHeader& h) {
				// Safely convert char array to string, stripping trailing null bytes
				std::string s(reinterpret_cast<const char*>(h.m_cComment), 16);
				s.erase(std::find(s.begin(), s.end(), '\0'), s.end());
				return s;
			},
			DOC_m_cComment);

	py::class_<BeamObject>(m, "BeamObject")
		.def_readonly("cfg", &BeamObject::cfg)
		.def_readonly("BeamData",
					  &BeamObject::BeamData);  // Automatically converts to nested Python lists

	m.def("read_data", &Read_data, "Parses a .raw radar file and returns a list of BeamObjects",
		  py::arg("filepath"));	 // Pull in your structs and function declaration
}