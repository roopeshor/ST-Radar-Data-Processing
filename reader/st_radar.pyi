"""
Python bindings for ST Radar raw data parser
"""
from __future__ import annotations
__all__: list[str] = ['BeamObject', 'RawBeamHeader', 'read_data']
class BeamObject:
    @property
    def BeamData(self) -> list[list[list[complex]]]:
        ...
    @property
    def cfg(self) -> RawBeamHeader:
        ...
class RawBeamHeader:
    @property
    def m_cArrRemarks(self) -> int:
        """
        ASCII strings for operator notes and experiment naming. 
        """
    @property
    def m_cComment(self) -> str:
        """
        ASCII strings for operator notes and experiment naming. 
        """
    @property
    def m_cReserved(self) -> int:
        """
        placeholders for future software updates. 
        """
    @property
    def m_fAltitude(self) -> float:
        """
        Geographic Altitude 
        """
    @property
    def m_fAzimuth(self) -> float:
        """
        The azimuthal pointing direction of the current beam.
        """
    @property
    def m_fBaudLength_us(self) -> float:
        """
        If the pulse is phase-coded (compressed), the pulse is divided into smaller "bauds" or "chips". The baud length dictates the actual range resolution (ΔR=c⋅baud/2), while the longer total pulse width maintains high average power.
        """
    @property
    def m_fIntrPulsePeriod_us(self) -> float:
        """
        (IPP): Inter-Pulse Period. The time between consecutive transmitted pulses. It determines the maximum unambiguous range (Rmax​=c⋅IPP/2) and the maximum measurable Doppler velocity (Nyquist limit).
        """
    @property
    def m_fLatitude(self) -> float:
        """
        Geographic latitude 
        """
    @property
    def m_fLongitude(self) -> float:
        """
        Geographic Longitude 
        """
    @property
    def m_fMGC_Gain_dB(self) -> float:
        """
        Manual Gain Control. The analog amplification applied to the receiver chain before ADC conversion to prevent saturation from ground clutter.
        """
    @property
    def m_fOffZenith(self) -> float:
        """
        The off-zeneith pointing direction of the current beam. Off-Zenith is the elevation angle relative to straight up (usually around 10° to 15° for ST atmospheric profiling).
        """
    @property
    def m_fPulseWidth_us(self) -> float:
        """
        The total duration of the transmitted RF pulse. Dictates the total transmitted energy.
        """
    @property
    def m_fRadiatedPower_ClusterWise(self) -> list[float]:
        """
        The ACARR radar array is divided into sub-arrays or "clusters" of antennas. This array tracks the power output of each specific hardware cluster for diagnostics and beam-forming calibration.
        """
    @property
    def m_fTRP_RF_Delay_us(self) -> float:
        """
        Transmit/Receive Path Delay. The internal hardware time it takes for the signal to travel through cables, T/R switches, and filters. This must be subtracted from the time-of-flight to accurately calculate "Altitude Zero".
        """
    @property
    def m_fTotalPowerRadiated(self) -> float:
        """
        Total transmit power. 
        """
    @property
    def m_fWindow1StartHeight(self) -> float:
        """
        The altitude boundaries (in km or meters) for up to 5 disjoint observation windows 
        """
    @property
    def m_sCodeFlag(self) -> int:
        """
        A boolean/integer flag indicating whether phase coding (pulse compression) is enabled.
        """
    @property
    def m_sCurrentBeamCnt(self) -> int:
        """
        Tracks the current scan beam cycle ST radars typically operate in DBS (Doppler Beam Swinging) mode, cycling through several beam directions (e.g., Zenith, North, East, South, West) to resolve the 3D wind vector.
        """
    @property
    def m_sNFFT(self) -> int:
        """
        Number of points in the Fast Fourier Transform. Determines the velocity (Doppler) resolution of the final spectra.
        """
    @property
    def m_sNumOfCohIntegrations(self) -> int:
        """
        Coherent Integrations. The number of raw I/Q voltage samples summed together before the FFT. This dramatically improves Signal-to-Noise Ratio (SNR) for slow-moving atmospheric targets and acts as a low-pass filter, reducing the effective sampling rate.
        """
    @property
    def m_sNumOfInCohIntegrations(self) -> int:
        """
        Incoherent Integrations. The number of power spectra (magnitudes) averaged together after the FFT. This does not change resolution but smooths the noise floor, making weak atmospheric echoes easier to detect.
        """
    @property
    def m_sNumOfObservWindows(self) -> int:
        """
        The radar can be programmed to only record specific altitude blocks to save bandwidth (e.g., sampling the troposphere, skipping the empty stratosphere, and sampling the mesosphere).
        """
    @property
    def m_sNumOfRangeBins(self) -> int:
        """
        The total number of altitude slices (gates) sampled along the beam path.
        """
    @property
    def m_sOperationMode(self) -> int:
        """
        Identifier for the operational mode (e.g., DBS wind profiling, spaced-antenna drift, meteor tracking).
        """
    @property
    def m_sReserved1(self) -> int:
        """
        placeholders for future updates or padding 
        """
    @property
    def m_sReserved4(self) -> int:
        """
        placeholders for future updates or padding 
        """
    @property
    def m_sStationID(self) -> int:
        """
        network identification. 
        """
    @property
    def m_sTotalNumberofBeams(self) -> int:
        """
        Number of beams ST radars typically operate in DBS (Doppler Beam Swinging) mode, cycling through several beam directions (e.g., Zenith, North, East, South, West) to resolve the 3D wind vector.
        """
    @property
    def m_sYear(self) -> int:
        """
        Timestamp in which first pulse in this specific coherent integration block was captured 
        """
    @property
    def m_ucExperimentDataEnable(self) -> int:
        """
        Flag distinguishing between actual atmospheric measurement runs and internal hardware calibration/noise-floor sampling.
        """
    @property
    def m_ulCodeA(self) -> list[int]:
        """
        The actual binary sequences used for phase modulation. Radars often use complementary code pairs (Code A and Code B) transmitted on alternating pulses to cancel out range sidelobes perfectly.
        """
    @property
    def m_ulCodeB(self) -> list[int]:
        """
        The actual binary sequences used for phase modulation. Radars often use complementary code pairs (Code A and Code B) transmitted on alternating pulses to cancel out range sidelobes perfectly.
        """
    @property
    def m_usCodeLength(self) -> int:
        """
        number of bits in the phase code (e.g., 16-bit or 32-bit code).
        """
    @property
    def m_usReserved(self) -> int:
        """
        placeholders for future updates or padding 
        """
    @property
    def m_usWindowType(self) -> int:
        """
        The mathematical window function (e.g., Hanning, Hamming, Blackman) applied to the time-domain data before the FFT to reduce spectral leakage.
        """
def read_data(filepath: str) -> list[BeamObject]:
    """
    Parses a .raw radar file and returns a list of BeamObjects
    """
