"""
Python bindings for ST Radar raw data parser
"""
from __future__ import annotations
__all__: list[str] = ['BeamObject', 'RawBeamHeader', 'read_data']
class BeamObject:
    """
    Extended structure to hold both header and the 3D data block
    """
    @property
    def BeamData(self) -> list[list[list[complex]]]:
        """
        Beam Data Dimensions: [inCohIntegrations][rangeBins][FFTPoints]
        """
    @property
    def direction(self) -> str:
        """
        Directinon in which beam is aligned Possible values: Vertical - North - West - South - East - Unknown
        """
    @property
    def header(self) -> RawBeamHeader:
        """
        Beam Header data
        """
    @property
    def maxVelocity(self) -> float:
        """
        Maximum Unambiguous Doppler Velocity / Nyquist velocity. If the wind blows faster than this limit, the signal aliases over to the opposite side of the spectrum It is given by: wavelength / (4 Teff), where Teff is effective time between each samples: Teff =  IPP * numOfCohIntegrations
        """
    @property
    def rangeResolution(self) -> float:
        """
        Resolution of each height. The radar pulse has to travel to the target and bounce back, this creates the minimum resolvable range. Range Resolution (ΔR) is: c * pulseDuration / 2
        """
class RawBeamHeader:
    """
    732 byte Beam header
    """
    @property
    def FFTPoints(self) -> int:
        """
        Number of points in the Fast Fourier Transform. Determines the velocity (Doppler) resolution of the final spectra. Formally: m_sNFFT
        """
    @property
    def IIP_us(self) -> float:
        """
        (IPP): Inter-Pulse Period (in microseconds). The time between consecutive transmitted pulses. It determines the maximum unambiguous range (Rmax​=c⋅IPP/2) and the maximum measurable Doppler velocity (Nyquist limit). Formally: m_fIntrPulsePeriod_us
        """
    @property
    def MGC_Gain_dB(self) -> float:
        """
        Manual Gain Control in dB. The analog amplification applied to the receiver chain before ADC conversion to prevent saturation from ground clutter.
        """
    @property
    def PMCodeA(self) -> list[int]:
        """
        The actual binary sequences used for phase modulation. Radars often use complementary code pairs (Code A and Code B) transmitted on alternating pulses to cancel out range sidelobes perfectly. Formally: m_ulCodeA
        """
    @property
    def PMCodeB(self) -> list[int]:
        """
        The actual binary sequences used for phase modulation. Radars often use complementary code pairs (Code A and Code B) transmitted on alternating pulses to cancel out range sidelobes perfectly. Formally: m_ulCodeB
        """
    @property
    def TRP_RF_Delay_us(self) -> float:
        """
        Transmit/Receive Path Delay in microseconds. The internal hardware time it takes for the signal to travel through cables, T/R switches, and filters. This must be subtracted from the time-of-flight to accurately calculate "Altitude Zero".
        """
    @property
    def altitude(self) -> float:
        """
        Geographic Altitude 
        """
    @property
    def azimuth(self) -> float:
        """
        The azimuthal pointing direction of the current beam.
        """
    @property
    def baudLength_us(self) -> float:
        """
        If the pulse is phase-coded (compressed), the pulse is divided into smaller "bauds" or "chips". The baud length dictates the actual range resolution in microseconds (ΔR=c⋅baud/2), while the longer total pulse width maintains high average power.
        """
    @property
    def beamCount(self) -> int:
        """
        Number of beams ST radars typically operate in DBS (Doppler Beam Swinging) mode, cycling through several beam directions (e.g., Zenith, North, East, South, West) to resolve the 3D wind vector. formally: m_sTotalNumberofBeams
        """
    @property
    def clusterWiseRadiatedPower(self) -> list[float]:
        """
        The ACARR radar array is divided into sub-arrays or "clusters" of antennas. This array tracks the power output of each specific hardware cluster for diagnostics and beam-forming calibration. formally: m_fRadiatedPower_ClusterWise
        """
    @property
    def codeFlag(self) -> int:
        """
        A boolean/integer flag indicating whether phase coding (pulse compression) is enabled. Formally: m_sCodeFlag
        """
    @property
    def codeLength(self) -> int:
        """
        number of bits in the phase code. Unsigned 16-bit code. Formally: m_usCodeLength
        """
    @property
    def comment(self) -> str:
        """
        ASCII strings for operator notes and experiment naming. 
        """
    @property
    def currentBeamCount(self) -> int:
        """
        Tracks the current scan beam cycle ST radars typically operate in DBS (Doppler Beam Swinging) mode, cycling through several beam directions (e.g., Zenith, North, East, South, West) to resolve the 3D wind vector. formally: m_sCurrentBeamCnt
        """
    @property
    def dayCaptured(self) -> int:
        """
        Day in which this coherent integration block was captured Formally: m_sDay
        """
    @property
    def experimentDataEnabled(self) -> int:
        """
        Flag distinguishing between actual atmospheric measurement runs and internal hardware calibration/noise-floor sampling. Formally: m_ucExperimentDataEnable
        """
    @property
    def hourCaptured(self) -> int:
        """
        Hour in which first pulse in this specific coherent integration block was captured Formally: m_sHour
        """
    @property
    def inCohIntegrations(self) -> int:
        """
        Incoherent Integrations. The number of power spectra (magnitudes) averaged together after the FFT. This does not change resolution but smooths the noise floor, making weak atmospheric echoes easier to detect.
        """
    @property
    def latitude(self) -> float:
        """
        Geographic latitude 
        """
    @property
    def longitude(self) -> float:
        """
        Geographic Longitude 
        """
    @property
    def minCaptured(self) -> int:
        """
        Min in which first pulse in this specific coherent integration block was captured Formally: m_sMin
        """
    @property
    def monthCaptured(self) -> int:
        """
        Month in which this coherent integration block was captured Formally: m_sMonth
        """
    @property
    def numOfCohIntegrations(self) -> int:
        """
        Coherent Integrations. The number of raw I/Q voltage samples summed together before the FFT. This dramatically improves Signal-to-Noise Ratio (SNR) for slow-moving atmospheric targets and acts as a low-pass filter, reducing the effective sampling rate.
        """
    @property
    def numOfObservedWindows(self) -> int:
        """
        The radar can be programmed to only record specific altitude blocks to save bandwidth (e.g., sampling the troposphere, skipping the empty stratosphere, and sampling the mesosphere). formally: m_sNumOfObservWindows
        """
    @property
    def offZenith(self) -> float:
        """
        The off-zeneith pointing direction of the current beam. Off-Zenith is the elevation angle relative to straight up (usually around 10° to 15° for ST atmospheric profiling).
        """
    @property
    def operationMode(self) -> int:
        """
        Identifier for the operational mode (e.g., DBS wind profiling, spaced-antenna drift, meteor tracking).
        """
    @property
    def pulseWidth_us(self) -> float:
        """
        The total duration of the transmitted RF pulse in microseconds. Dictates the total transmitted energy. formally:  m_fPulseWidth_us
        """
    @property
    def rangeBins(self) -> int:
        """
        732 byte Beam header */ struct RawBeamHeader { int16_t fileMagicNumber; /** The total number of altitude slices (gates) sampled along the beam path.
        """
    @property
    def remarks(self) -> list[int]:
        """
        ASCII strings for operator notes and experiment naming. Formally: m_cArrRemarks
        """
    @property
    def reserved1(self) -> int:
        """
        placeholders for future updates or padding 
        """
    @property
    def reserved4(self) -> int:
        """
        placeholders for future updates or padding 
        """
    @property
    def reserved5(self) -> int:
        """
        placeholders for future updates or padding formally: m_usReserved
        """
    @property
    def reservedCharArr(self) -> list[int]:
        """
        placeholders for future software updates. Formally: m_cReserved
        """
    @property
    def secCaptured(self) -> int:
        """
        Second in which first pulse in this specific coherent integration block was captured Formally: m_sSec
        """
    @property
    def stationID(self) -> int:
        """
        network identification. 
        """
    @property
    def totalPowerRadiated(self) -> float:
        """
        Total transmit power. 
        """
    @property
    def window1EndHeight(self) -> float:
        """
        End of altitude boundaries (in km or meters) of 1st observation windows 
        """
    @property
    def window1StartHeight(self) -> float:
        """
        Start of altitude boundaries (in km or meters) of 1st observation windows 
        """
    @property
    def window2EndHeight(self) -> float:
        """
        End of altitude boundaries (in km or meters) of 2st observation windows 
        """
    @property
    def window2StartHeight(self) -> float:
        """
        Start of altitude boundaries (in km or meters) of 2st observation windows 
        """
    @property
    def window3EndHeight(self) -> float:
        """
        End of altitude boundaries (in km or meters) of 3st observation windows 
        """
    @property
    def window3StartHeight(self) -> float:
        """
        Start of altitude boundaries (in km or meters) of 3st observation windows 
        """
    @property
    def window4EndHeight(self) -> float:
        """
        End of altitude boundaries (in km or meters) of 4st observation windows 
        """
    @property
    def window4StartHeight(self) -> float:
        """
        Start of altitude boundaries (in km or meters) of 4st observation windows 
        """
    @property
    def window5EndHeight(self) -> float:
        """
        End of altitude boundaries (in km or meters) of 5st observation windows 
        """
    @property
    def window5StartHeight(self) -> float:
        """
        Start of altitude boundaries (in km or meters) of 5st observation windows 
        """
    @property
    def windowType(self) -> int:
        """
        The mathematical window function (e.g., Hanning, Hamming, Blackman) applied to the time-domain data before the FFT to reduce spectral leakage. formally: m_usWindowType
        """
    @property
    def yearCaptured(self) -> int:
        """
        Year in which this coherent integration block was captured 
        """
def read_data(filepath: str) -> list[BeamObject]:
    """
    Takes .raw radar file path, parses it and returns a list of BeamObjects
    """
