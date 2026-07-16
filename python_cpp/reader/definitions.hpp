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

using complex32 = std::complex<int32_t> ;
using ComplexDouble = std::complex<double> ;
using RadarCube = vec<vec<vec<ComplexDouble>>>;	 // [InCoh][RangeBins][NFFT]
using PowerSpectrum = vec<vec<double>>;			 // [RangeBins][NFFT]

const int16_t DEFAULT_MAGIC_NUMBER = 369;
const size_t BEAM_HEADER_SIZE = 732;

#pragma pack(push, 1)

/** 732 byte Beam header */
struct RawBeamHeader {
	int16_t fileMagicNumber;

	/**
	 * The total number of altitude slices (gates) sampled along the beam path.
	 */
	int16_t rangeBins;

	/**
	 * If the pulse is phase-coded (compressed), the pulse is divided into smaller
	 * "bauds" or "chips". The baud length dictates the actual range resolution in microseconds
	 * (ΔR=c⋅baud/2), while the longer total pulse width maintains high average power.
	 */
	float baudLength_us;

	/**
	 * Number of points in the Fast Fourier Transform. Determines the velocity (Doppler)
	 * resolution of the final spectra.
	 * Formally: m_sNFFT
	 */
	int16_t FFTPoints;

	/**
	 * Coherent Integrations. The number of raw I/Q voltage samples summed together before
	 * the FFT. This dramatically improves Signal-to-Noise Ratio (SNR) for slow-moving
	 * atmospheric targets and acts as a low-pass filter, reducing the effective sampling
	 * rate.
	 */
	int16_t numOfCohIntegrations;

	/**
	 * Incoherent Integrations. The number of power spectra (magnitudes) averaged together
	 * after the FFT. This does not change resolution but smooths the noise floor, making
	 * weak atmospheric echoes easier to detect.
	 */
	int16_t inCohIntegrations;

	/**
	 * A boolean/integer flag indicating whether phase coding (pulse compression) is
	 * enabled.
	 * Formally: m_sCodeFlag
	 */
	int16_t codeFlag;

	/**
	 * (IPP): Inter-Pulse Period (in microseconds). The time between consecutive transmitted pulses.
	 * It determines the maximum unambiguous range (Rmax​=c⋅IPP/2) and the maximum
	 * measurable Doppler velocity (Nyquist limit).
	 * Formally: m_fIntrPulsePeriod_us
	 */
	float IIP_us;

	/**
	 * The total duration of the transmitted RF pulse in microseconds. Dictates the total
	 * transmitted energy. formally:  m_fPulseWidth_us
	 */
	float pulseWidth_us;

	/**
	 * The radar can be programmed to only record specific altitude blocks to save
	 * bandwidth (e.g., sampling the troposphere, skipping the empty stratosphere, and
	 * sampling the mesosphere).
	 * formally: m_sNumOfObservWindows
	 */
	int16_t numOfObservedWindows;

	/** network identification. */
	int16_t stationID;

	/** placeholders for future updates or padding */
	int16_t reserved1;
	int16_t reserved2;
	int16_t reserved3;

	/** Year in which this coherent integration block was captured */
	int16_t yearCaptured;
	/**
	 * Month in which this coherent integration block was captured
	 * Formally: m_sMonth
	 */
	int16_t monthCaptured;
	/**
	 * Day in which this coherent integration block was captured
	 * Formally: m_sDay
	 */
	int16_t dayCaptured;
	/**
	 * Hour in which first pulse in this specific coherent integration block was captured
	 * Formally: m_sHour
	 */
	int16_t hourCaptured;
	/**
	 * Min in which first pulse in this specific coherent integration block was captured
	 * Formally: m_sMin
	 */
	int16_t minCaptured;
	/**
	 * Second in which first pulse in this specific coherent integration block was captured
	 * Formally: m_sSec
	 */
	int16_t secCaptured;

	/** placeholders for future updates or padding */
	int16_t reserved4;

	/** Geographic latitude */
	float latitude;
	/** Geographic Longitude */
	float longitude;
	/** Geographic Altitude */
	float altitude;

	/** Total transmit power. */
	float totalPowerRadiated;

	/**
	 * The ACARR radar array is divided into sub-arrays or "clusters" of antennas. This
	 * array tracks the power output of each specific hardware cluster for diagnostics and
	 * beam-forming calibration.
	 * formally: m_fRadiatedPower_ClusterWise
	 */
	float clusterWiseRadiatedPower[13];

	/**
	 * Identifier for the operational mode (e.g., DBS wind profiling, spaced-antenna
	 * drift, meteor tracking).
	 */
	int16_t operationMode;

	/**
	 * Tracks the current scan beam cycle
	 * ST radars typically operate in DBS (Doppler Beam Swinging) mode, cycling through
	 * several beam directions (e.g., Zenith, North, East, South, West) to resolve the 3D
	 * wind vector.
	 * formally: m_sCurrentBeamCnt
	 */
	int16_t currentBeamCount;
	/**
	 * Number of beams
	 * ST radars typically operate in DBS (Doppler Beam Swinging) mode, cycling through
	 * several beam directions (e.g., Zenith, North, East, South, West) to resolve the 3D
	 * wind vector.
	 * formally: m_sTotalNumberofBeams
	 */
	int16_t beamCount;

	/**
	 * number of bits in the phase code. Unsigned 16-bit code.
	 * Formally: m_usCodeLength
	 */
	int16_t codeLength;

	/** ASCII strings for operator notes and experiment naming. */
	int8_t comment[16];

	/**
	 * Transmit/Receive Path Delay in microseconds. The internal hardware time it takes for the
	 * signal to travel through cables, T/R switches, and filters. This must be subtracted from the
	 * time-of-flight to accurately calculate "Altitude Zero".
	 */
	float TRP_RF_Delay_us;

	/**
	 * Manual Gain Control in dB. The analog amplification applied to the receiver chain before
	 * ADC conversion to prevent saturation from ground clutter.
	 */
	float MGC_Gain_dB;

	/**
	 * The mathematical window function (e.g., Hanning, Hamming, Blackman) applied to the
	 * time-domain data before the FFT to reduce spectral leakage.
	 * formally: m_usWindowType
	 */
	int16_t windowType;

	/**
	 * placeholders for future updates or padding
	 * formally: m_usReserved
	 */
	int16_t reserved5;

	/**
	 * ASCII strings for operator notes and experiment naming.
	 * Formally: m_cArrRemarks
	 */
	int8_t remarks[512];

	/**
	 * The actual binary sequences used for phase modulation.
	 * Radars often use complementary code pairs (Code A and Code B)
	 * transmitted on alternating pulses to cancel out range sidelobes perfectly.
	 * Formally: m_ulCodeA
	 */
	uint32_t PMCodeA[2];
	/**
	 * The actual binary sequences used for phase modulation.
	 * Radars often use complementary code pairs (Code A and Code B)
	 * transmitted on alternating pulses to cancel out range sidelobes perfectly.
	 * Formally: m_ulCodeB
	 */
	uint32_t PMCodeB[2];

	/**
	 * Flag distinguishing between actual atmospheric measurement runs and internal
	 * hardware calibration/noise-floor sampling.
	 * Formally: m_ucExperimentDataEnable
	 */
	int8_t experimentDataEnabled;

	/**
	 * placeholders for future software updates.
	 * Formally: m_cReserved
	 */
	int8_t reservedCharArr[3];

	/**
	 * The azimuthal pointing direction of the current beam.
	 */
	float azimuth;
	/**
	 * The off-zeneith pointing direction of the current beam. Off-Zenith is the elevation angle
	 * relative to straight up (usually around 10° to 15° for ST atmospheric profiling).
	 */
	float offZenith;

	/** Start of altitude boundaries (in km or meters) of 1st observation windows */
	float window1StartHeight;

	/** End of altitude boundaries (in km or meters) of 1st observation windows */
	float window1EndHeight;

	/** Start of altitude boundaries (in km or meters) of 2st observation windows */
	float window2StartHeight;

	/** End of altitude boundaries (in km or meters) of 2st observation windows */
	float window2EndHeight;

	/** Start of altitude boundaries (in km or meters) of 3st observation windows */
	float window3StartHeight;

	/** End of altitude boundaries (in km or meters) of 3st observation windows */
	float window3EndHeight;

	/** Start of altitude boundaries (in km or meters) of 4st observation windows */
	float window4StartHeight;

	/** End of altitude boundaries (in km or meters) of 4st observation windows */
	float window4EndHeight;

	/** Start of altitude boundaries (in km or meters) of 5st observation windows */
	float window5StartHeight;

	/** End of altitude boundaries (in km or meters) of 5st observation windows */
	float window5EndHeight;
};
#pragma pack(pop)

/**
 * Extended structure to hold both header and the 3D data block
 */
struct BeamObject {
	/** Beam Header data */
	RawBeamHeader header;
	/**
	 * Beam Data
	 * Dimensions: [inCohIntegrations][rangeBins][FFTPoints]
	 */
	vec<vec<vec<complex32>>> BeamData;

	/**
	 * Resolution of each height. The radar pulse has to travel to the target and bounce back, this
	 * creates the minimum resolvable range. Range Resolution (ΔR) is: c * pulseDuration / 2
	 */
	float rangeResolution;

	/**
	 * Maximum Unambiguous Doppler Velocity / Nyquist velocity. If the wind blows faster than this
	 * limit, the signal aliases over to the opposite side of the spectrum
	 * It is given by: wavelength / (4 Teff), where Teff is effective time between each samples:
	 * Teff =  IPP * numOfCohIntegrations
	 */
	float maxVelocity;

	/**
	 * Directinon in which beam is aligned
	 * Possible values:
	 * Vertical
	 * - North
	 * - West
	 * - South
	 * - East
	 * - Unknown
	 */
	std::string direction;
};

vec<BeamObject> Read_data(const std::string& filepath);
float getRangeResolution(RawBeamHeader beamHeader);
std::string getBeamDirection(int azimuth, float offZenith);

#endif