// AUTO-GENERATED FILE. DO NOT EDIT.
#ifndef AUTO_DOCS_H
#define AUTO_DOCS_H

#pragma once

constexpr const char* DOC_rangeBins = "732 byte Beam header */ struct RawBeamHeader { int16_t fileMagicNumber; /** The total number of altitude slices (gates) sampled along the beam path.";
constexpr const char* DOC_baudLength_us = "If the pulse is phase-coded (compressed), the pulse is divided into smaller \"bauds\" or \"chips\". The baud length dictates the actual range resolution in microseconds (ΔR=c⋅baud/2), while the longer total pulse width maintains high average power.";
constexpr const char* DOC_FFTPoints = "Number of points in the Fast Fourier Transform. Determines the velocity (Doppler) resolution of the final spectra. Formally: m_sNFFT";
constexpr const char* DOC_numOfCohIntegrations = "Coherent Integrations. The number of raw I/Q voltage samples summed together before the FFT. This dramatically improves Signal-to-Noise Ratio (SNR) for slow-moving atmospheric targets and acts as a low-pass filter, reducing the effective sampling rate.";
constexpr const char* DOC_inCohIntegrations = "Incoherent Integrations. The number of power spectra (magnitudes) averaged together after the FFT. This does not change resolution but smooths the noise floor, making weak atmospheric echoes easier to detect.";
constexpr const char* DOC_codeFlag = "A boolean/integer flag indicating whether phase coding (pulse compression) is enabled. Formally: m_sCodeFlag";
constexpr const char* DOC_IIP_us = "(IPP): Inter-Pulse Period (in microseconds). The time between consecutive transmitted pulses. It determines the maximum unambiguous range (Rmax​=c⋅IPP/2) and the maximum measurable Doppler velocity (Nyquist limit). Formally: m_fIntrPulsePeriod_us";
constexpr const char* DOC_pulseWidth_us = "The total duration of the transmitted RF pulse in microseconds. Dictates the total transmitted energy. formally:  m_fPulseWidth_us";
constexpr const char* DOC_numOfObservedWindows = "The radar can be programmed to only record specific altitude blocks to save bandwidth (e.g., sampling the troposphere, skipping the empty stratosphere, and sampling the mesosphere). formally: m_sNumOfObservWindows";
constexpr const char* DOC_stationID = "network identification. ";
constexpr const char* DOC_reserved1 = "placeholders for future updates or padding ";
constexpr const char* DOC_yearCaptured = "Year in which this coherent integration block was captured ";
constexpr const char* DOC_monthCaptured = "Month in which this coherent integration block was captured Formally: m_sMonth";
constexpr const char* DOC_dayCaptured = "Day in which this coherent integration block was captured Formally: m_sDay";
constexpr const char* DOC_hourCaptured = "Hour in which first pulse in this specific coherent integration block was captured Formally: m_sHour";
constexpr const char* DOC_minCaptured = "Min in which first pulse in this specific coherent integration block was captured Formally: m_sMin";
constexpr const char* DOC_secCaptured = "Second in which first pulse in this specific coherent integration block was captured Formally: m_sSec";
constexpr const char* DOC_reserved4 = "placeholders for future updates or padding ";
constexpr const char* DOC_latitude = "Geographic latitude ";
constexpr const char* DOC_longitude = "Geographic Longitude ";
constexpr const char* DOC_altitude = "Geographic Altitude ";
constexpr const char* DOC_totalPowerRadiated = "Total transmit power. ";
constexpr const char* DOC_clusterWiseRadiatedPower = "The ACARR radar array is divided into sub-arrays or \"clusters\" of antennas. This array tracks the power output of each specific hardware cluster for diagnostics and beam-forming calibration. formally: m_fRadiatedPower_ClusterWise";
constexpr const char* DOC_operationMode = "Identifier for the operational mode (e.g., DBS wind profiling, spaced-antenna drift, meteor tracking).";
constexpr const char* DOC_currentBeamCount = "Tracks the current scan beam cycle ST radars typically operate in DBS (Doppler Beam Swinging) mode, cycling through several beam directions (e.g., Zenith, North, East, South, West) to resolve the 3D wind vector. formally: m_sCurrentBeamCnt";
constexpr const char* DOC_beamCount = "Number of beams ST radars typically operate in DBS (Doppler Beam Swinging) mode, cycling through several beam directions (e.g., Zenith, North, East, South, West) to resolve the 3D wind vector. formally: m_sTotalNumberofBeams";
constexpr const char* DOC_codeLength = "number of bits in the phase code. Unsigned 16-bit code. Formally: m_usCodeLength";
constexpr const char* DOC_comment = "ASCII strings for operator notes and experiment naming. ";
constexpr const char* DOC_TRP_RF_Delay_us = "Transmit/Receive Path Delay in microseconds. The internal hardware time it takes for the signal to travel through cables, T/R switches, and filters. This must be subtracted from the time-of-flight to accurately calculate \"Altitude Zero\".";
constexpr const char* DOC_MGC_Gain_dB = "Manual Gain Control in dB. The analog amplification applied to the receiver chain before ADC conversion to prevent saturation from ground clutter.";
constexpr const char* DOC_windowType = "The mathematical window function (e.g., Hanning, Hamming, Blackman) applied to the time-domain data before the FFT to reduce spectral leakage. formally: m_usWindowType";
constexpr const char* DOC_reserved5 = "placeholders for future updates or padding formally: m_usReserved";
constexpr const char* DOC_remarks = "ASCII strings for operator notes and experiment naming. Formally: m_cArrRemarks";
constexpr const char* DOC_PMCodeA = "The actual binary sequences used for phase modulation. Radars often use complementary code pairs (Code A and Code B) transmitted on alternating pulses to cancel out range sidelobes perfectly. Formally: m_ulCodeA";
constexpr const char* DOC_PMCodeB = "The actual binary sequences used for phase modulation. Radars often use complementary code pairs (Code A and Code B) transmitted on alternating pulses to cancel out range sidelobes perfectly. Formally: m_ulCodeB";
constexpr const char* DOC_experimentDataEnabled = "Flag distinguishing between actual atmospheric measurement runs and internal hardware calibration/noise-floor sampling. Formally: m_ucExperimentDataEnable";
constexpr const char* DOC_reservedCharArr = "placeholders for future software updates. Formally: m_cReserved";
constexpr const char* DOC_azimuth = "The azimuthal pointing direction of the current beam.";
constexpr const char* DOC_offZenith = "The off-zeneith pointing direction of the current beam. Off-Zenith is the elevation angle relative to straight up (usually around 10° to 15° for ST atmospheric profiling).";
constexpr const char* DOC_window1StartHeight = "Start of altitude boundaries (in km or meters) of 1st observation windows ";
constexpr const char* DOC_window1EndHeight = "End of altitude boundaries (in km or meters) of 1st observation windows ";
constexpr const char* DOC_window2StartHeight = "Start of altitude boundaries (in km or meters) of 2st observation windows ";
constexpr const char* DOC_window2EndHeight = "End of altitude boundaries (in km or meters) of 2st observation windows ";
constexpr const char* DOC_window3StartHeight = "Start of altitude boundaries (in km or meters) of 3st observation windows ";
constexpr const char* DOC_window3EndHeight = "End of altitude boundaries (in km or meters) of 3st observation windows ";
constexpr const char* DOC_window4StartHeight = "Start of altitude boundaries (in km or meters) of 4st observation windows ";
constexpr const char* DOC_window4EndHeight = "End of altitude boundaries (in km or meters) of 4st observation windows ";
constexpr const char* DOC_window5StartHeight = "Start of altitude boundaries (in km or meters) of 5st observation windows ";
constexpr const char* DOC_window5EndHeight = "End of altitude boundaries (in km or meters) of 5st observation windows ";
constexpr const char* DOC_header = "Extended structure to hold both header and the 3D data block / struct BeamObject { /** Beam Header data ";
constexpr const char* DOC_BeamData = "Beam Data Dimensions: [inCohIntegrations][rangeBins][FFTPoints]";
constexpr const char* DOC_rangeResolution = "Resolution of each height. The radar pulse has to travel to the target and bounce back, this creates the minimum resolvable range. Range Resolution (ΔR) is: c * pulseDuration / 2";
constexpr const char* DOC_maxVelocity = "Maximum Unambiguous Doppler Velocity / Nyquist velocity. If the wind blows faster than this limit, the signal aliases over to the opposite side of the spectrum It is given by: wavelength / (4 Teff), where Teff is effective time between each samples: Teff =  IPP * numOfCohIntegrations";
constexpr const char* DOC_direction = "Directinon in which beam is aligned Possible values: Vertical - North - West - South - East - Unknown";
#endif