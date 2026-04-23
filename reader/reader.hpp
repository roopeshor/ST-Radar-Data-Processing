#ifndef READER_H
#define READER_H
#include <complex>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

template <typename T>
using vec = std::vector<T>;
typedef std::complex<int32_t> complex;

#pragma pack(push, 1)
struct RawBeamHeader {
	int16_t m_sMagicNumber;

	/**
	 * The total number of altitude slices (gates) sampled along the beam path.
	 */
	int16_t m_sNumOfRangeBins;

	/**
	 * If the pulse is phase-coded (compressed), the pulse is divided into smaller
	 * "bauds" or "chips". The baud length dictates the actual range resolution
	 * (ΔR=c⋅baud/2), while the longer total pulse width maintains high average power.
	 */
	float m_fBaudLength_us;

	/**
	 * Number of points in the Fast Fourier Transform. Determines the velocity (Doppler)
	 * resolution of the final spectra.
	 */
	int16_t m_sNFFT;

	/**
	 * Coherent Integrations. The number of raw I/Q voltage samples summed together before
	 * the FFT. This dramatically improves Signal-to-Noise Ratio (SNR) for slow-moving
	 * atmospheric targets and acts as a low-pass filter, reducing the effective sampling
	 * rate.
	 */
	int16_t m_sNumOfCohIntegrations;

	/**
	 * Incoherent Integrations. The number of power spectra (magnitudes) averaged together
	 * after the FFT. This does not change resolution but smooths the noise floor, making
	 * weak atmospheric echoes easier to detect.
	 */
	int16_t m_sNumOfInCohIntegrations;

	/**
	 * A boolean/integer flag indicating whether phase coding (pulse compression) is
	 * enabled.
	 */
	int16_t m_sCodeFlag;

	/**
	 * (IPP): Inter-Pulse Period. The time between consecutive transmitted pulses.
	 * It determines the maximum unambiguous range (Rmax​=c⋅IPP/2) and the maximum
	 * measurable Doppler velocity (Nyquist limit).
	 */
	float m_fIntrPulsePeriod_us;

	/**
	 * The total duration of the transmitted RF pulse. Dictates the total transmitted
	 * energy.
	 */
	float m_fPulseWidth_us;

	/**
	 * The radar can be programmed to only record specific altitude blocks to save
	 * bandwidth (e.g., sampling the troposphere, skipping the empty stratosphere, and
	 * sampling the mesosphere).
	 */
	int16_t m_sNumOfObservWindows;

	/** network identification. */
	int16_t m_sStationID;

	/** placeholders for future updates or padding */
	int16_t m_sReserved1;
	int16_t m_sReserved2;
	int16_t m_sReserved3;

	/** Timestamp in which first pulse in this specific coherent integration block was captured */
	int16_t m_sYear;
	int16_t m_sMonth;
	int16_t m_sDay;
	int16_t m_sHour;
	int16_t m_sMin;
	int16_t m_sSec;

	/** placeholders for future updates or padding */
	int16_t m_sReserved4;

	/** Geographic latitude */
	float m_fLatitude;
	/** Geographic Longitude */
	float m_fLongitude;
	/** Geographic Altitude */
	float m_fAltitude;

	/** Total transmit power. */
	float m_fTotalPowerRadiated;

	/**
	 * The ACARR radar array is divided into sub-arrays or "clusters" of antennas. This
	 * array tracks the power output of each specific hardware cluster for diagnostics and
	 * beam-forming calibration.
	 */
	float m_fRadiatedPower_ClusterWise[13];

	/**
	 * Identifier for the operational mode (e.g., DBS wind profiling, spaced-antenna
	 * drift, meteor tracking).
	 */
	int16_t m_sOperationMode;

	/**
	 * Tracks the current scan beam cycle
	 * ST radars typically operate in DBS (Doppler Beam Swinging) mode, cycling through
	 * several beam directions (e.g., Zenith, North, East, South, West) to resolve the 3D
	 * wind vector.
	 */
	int16_t m_sCurrentBeamCnt;
	/**
	 * Number of beams
	 * ST radars typically operate in DBS (Doppler Beam Swinging) mode, cycling through
	 * several beam directions (e.g., Zenith, North, East, South, West) to resolve the 3D
	 * wind vector.
	 */
	int16_t m_sTotalNumberofBeams;

	/**
	 * number of bits in the phase code (e.g., 16-bit or 32-bit code).
	 */
	int16_t m_usCodeLength;

	/** ASCII strings for operator notes and experiment naming. */
	int8_t m_cComment[16];

	/**
	 * Transmit/Receive Path Delay. The internal hardware time it takes for the signal to
	 * travel through cables, T/R switches, and filters. This must be subtracted from the
	 * time-of-flight to accurately calculate "Altitude Zero".
	 */
	float m_fTRP_RF_Delay_us;

	/**
	 * Manual Gain Control. The analog amplification applied to the receiver chain before
	 * ADC conversion to prevent saturation from ground clutter.
	 */
	float m_fMGC_Gain_dB;

	/**
	 * The mathematical window function (e.g., Hanning, Hamming, Blackman) applied to the
	 * time-domain data before the FFT to reduce spectral leakage.
	 */
	int16_t m_usWindowType;

	/** placeholders for future updates or padding */
	int16_t m_usReserved;

	/** ASCII strings for operator notes and experiment naming. */
	int8_t m_cArrRemarks[512];

	/**
	 * The actual binary sequences used for phase modulation.
	 * Radars often use complementary code pairs (Code A and Code B)
	 * transmitted on alternating pulses to cancel out range sidelobes perfectly.
	 */
	uint32_t m_ulCodeA[2];
	/**
	 * The actual binary sequences used for phase modulation.
	 * Radars often use complementary code pairs (Code A and Code B)
	 * transmitted on alternating pulses to cancel out range sidelobes perfectly.
	 */
	uint32_t m_ulCodeB[2];

	/**
	 * Flag distinguishing between actual atmospheric measurement runs and internal
	 * hardware calibration/noise-floor sampling.
	 */
	int8_t m_ucExperimentDataEnable;

	/** placeholders for future software updates. */
	int8_t m_cReserved[3];

	/**
	 * The azimuthal pointing direction of the current beam.
	 */
	float m_fAzimuth;
	/**
	 * The off-zeneith pointing direction of the current beam. Off-Zenith is the elevation angle
	 * relative to straight up (usually around 10° to 15° for ST atmospheric profiling).
	 */
	float m_fOffZenith;

	/** The altitude boundaries (in km or meters) for up to 5 disjoint observation windows */
	float m_fWindow1StartHeight;
	float m_fWindow1EndHeight;
	float m_fWindow2StartHeight;
	float m_fWindow2EndHeight;
	float m_fWindow3StartHeight;
	float m_fWindow3EndHeight;
	float m_fWindow4StartHeight;
	float m_fWindow4EndHeight;
	float m_fWindow5StartHeight;
	float m_fWindow5EndHeight;
};
#pragma pack(pop)

// Extended structure to hold both header and the 3D data block
struct BeamObject {
	RawBeamHeader cfg;
	// Dimensions: [InCohIntegration][RangeBin][NFFT]
	vec<vec<vec<complex>>> BeamData;
};

vec<BeamObject> Read_data(const std::string& filepath);

#endif