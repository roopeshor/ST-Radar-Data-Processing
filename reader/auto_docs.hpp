// AUTO-GENERATED FILE. DO NOT EDIT.
#ifndef AUTO_DOCS_H
#define AUTO_DOCS_H

#pragma once

constexpr const char* DOC_m_sNumOfRangeBins = "The total number of altitude slices (gates) sampled along the beam path.";
constexpr const char* DOC_m_fBaudLength_us = "If the pulse is phase-coded (compressed), the pulse is divided into smaller \"bauds\" or \"chips\". The baud length dictates the actual range resolution (ΔR=c⋅baud/2), while the longer total pulse width maintains high average power.";
constexpr const char* DOC_m_sNFFT = "Number of points in the Fast Fourier Transform. Determines the velocity (Doppler) resolution of the final spectra.";
constexpr const char* DOC_m_sNumOfCohIntegrations = "Coherent Integrations. The number of raw I/Q voltage samples summed together before the FFT. This dramatically improves Signal-to-Noise Ratio (SNR) for slow-moving atmospheric targets and acts as a low-pass filter, reducing the effective sampling rate.";
constexpr const char* DOC_m_sNumOfInCohIntegrations = "Incoherent Integrations. The number of power spectra (magnitudes) averaged together after the FFT. This does not change resolution but smooths the noise floor, making weak atmospheric echoes easier to detect.";
constexpr const char* DOC_m_sCodeFlag = "A boolean/integer flag indicating whether phase coding (pulse compression) is enabled.";
constexpr const char* DOC_m_fIntrPulsePeriod_us = "(IPP): Inter-Pulse Period. The time between consecutive transmitted pulses. It determines the maximum unambiguous range (Rmax​=c⋅IPP/2) and the maximum measurable Doppler velocity (Nyquist limit).";
constexpr const char* DOC_m_fPulseWidth_us = "The total duration of the transmitted RF pulse. Dictates the total transmitted energy.";
constexpr const char* DOC_m_sNumOfObservWindows = "The radar can be programmed to only record specific altitude blocks to save bandwidth (e.g., sampling the troposphere, skipping the empty stratosphere, and sampling the mesosphere).";
constexpr const char* DOC_m_sStationID = "network identification. ";
constexpr const char* DOC_m_sReserved1 = "placeholders for future updates or padding ";
constexpr const char* DOC_m_sYear = "Timestamp in which first pulse in this specific coherent integration block was captured ";
constexpr const char* DOC_m_sReserved4 = "placeholders for future updates or padding ";
constexpr const char* DOC_m_fLatitude = "Geographic latitude ";
constexpr const char* DOC_m_fLongitude = "Geographic Longitude ";
constexpr const char* DOC_m_fAltitude = "Geographic Altitude ";
constexpr const char* DOC_m_fTotalPowerRadiated = "Total transmit power. ";
constexpr const char* DOC_m_fRadiatedPower_ClusterWise = "The ACARR radar array is divided into sub-arrays or \"clusters\" of antennas. This array tracks the power output of each specific hardware cluster for diagnostics and beam-forming calibration.";
constexpr const char* DOC_m_sOperationMode = "Identifier for the operational mode (e.g., DBS wind profiling, spaced-antenna drift, meteor tracking).";
constexpr const char* DOC_m_sCurrentBeamCnt = "Tracks the current scan beam cycle ST radars typically operate in DBS (Doppler Beam Swinging) mode, cycling through several beam directions (e.g., Zenith, North, East, South, West) to resolve the 3D wind vector.";
constexpr const char* DOC_m_sTotalNumberofBeams = "Number of beams ST radars typically operate in DBS (Doppler Beam Swinging) mode, cycling through several beam directions (e.g., Zenith, North, East, South, West) to resolve the 3D wind vector.";
constexpr const char* DOC_m_usCodeLength = "number of bits in the phase code (e.g., 16-bit or 32-bit code).";
constexpr const char* DOC_m_cComment = "ASCII strings for operator notes and experiment naming. ";
constexpr const char* DOC_m_fTRP_RF_Delay_us = "Transmit/Receive Path Delay. The internal hardware time it takes for the signal to travel through cables, T/R switches, and filters. This must be subtracted from the time-of-flight to accurately calculate \"Altitude Zero\".";
constexpr const char* DOC_m_fMGC_Gain_dB = "Manual Gain Control. The analog amplification applied to the receiver chain before ADC conversion to prevent saturation from ground clutter.";
constexpr const char* DOC_m_usWindowType = "The mathematical window function (e.g., Hanning, Hamming, Blackman) applied to the time-domain data before the FFT to reduce spectral leakage.";
constexpr const char* DOC_m_usReserved = "placeholders for future updates or padding ";
constexpr const char* DOC_m_cArrRemarks = "ASCII strings for operator notes and experiment naming. ";
constexpr const char* DOC_m_ulCodeA = "The actual binary sequences used for phase modulation. Radars often use complementary code pairs (Code A and Code B) transmitted on alternating pulses to cancel out range sidelobes perfectly.";
constexpr const char* DOC_m_ulCodeB = "The actual binary sequences used for phase modulation. Radars often use complementary code pairs (Code A and Code B) transmitted on alternating pulses to cancel out range sidelobes perfectly.";
constexpr const char* DOC_m_ucExperimentDataEnable = "Flag distinguishing between actual atmospheric measurement runs and internal hardware calibration/noise-floor sampling.";
constexpr const char* DOC_m_cReserved = "placeholders for future software updates. ";
constexpr const char* DOC_m_fAzimuth = "The azimuthal pointing direction of the current beam.";
constexpr const char* DOC_m_fOffZenith = "The off-zeneith pointing direction of the current beam. Off-Zenith is the elevation angle relative to straight up (usually around 10° to 15° for ST atmospheric profiling).";
constexpr const char* DOC_m_fWindow1StartHeight = "The altitude boundaries (in km or meters) for up to 5 disjoint observation windows ";
#endif