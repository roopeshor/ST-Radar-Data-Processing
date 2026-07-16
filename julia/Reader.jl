module Reader

export RawBeamHeader, BeamObject, read_data, getRangeResolution, getBeamDirection

"""
732 byte Beam header
"""
struct RawBeamHeader
    fileMagicNumber::Int16

    """
    The total number of altitude slices (gates) sampled along the beam path.
    """
    rangeBins::Int16

    """
    If the pulse is phase-coded (compressed), the pulse is divided into smaller
    "bauds" or "chips". The baud length dictates the actual range resolution in microseconds
    (ΔR=c⋅baud/2), while the longer total pulse width maintains high average power.
    """
    baudLength_us::Float32

    """
    Number of points in the Fast Fourier Transform. Determines the velocity (Doppler)
    resolution of the final spectra.
    """
    FFTPoints::Int16

    """
    Coherent Integrations. The number of raw I/Q voltage samples summed together before
    the FFT. This dramatically improves Signal-to-Noise Ratio (SNR) for slow-moving
    atmospheric targets and acts as a low-pass filter, reducing the effective sampling
    rate.
    """
    numOfCohIntegrations::Int16

    """
    Incoherent Integrations. The number of power spectra (magnitudes) averaged together
    after the FFT. This does not change resolution but smooths the noise floor, making
    weak atmospheric echoes easier to detect.
    """
    inCohIntegrations::Int16

    """
    A boolean/integer flag indicating whether phase coding (pulse compression) is
    enabled.
    """
    codeFlag::Int16

    """
    (IPP): Inter-Pulse Period (in microseconds). The time between consecutive transmitted pulses.
    It determines the maximum unambiguous range (Rmax​=c⋅IPP/2) and the maximum
    measurable Doppler velocity (Nyquist limit).
    """
    IIP_us::Float32

    """
    The total duration of the transmitted RF pulse in microseconds. Dictates the total
    transmitted energy.
    """
    pulseWidth_us::Float32

    """
    The radar can be programmed to only record specific altitude blocks to save
    bandwidth (e.g., sampling the troposphere, skipping the empty stratosphere, and
    sampling the mesosphere).
    """
    numOfObservedWindows::Int16

    """network identification."""
    stationID::Int16

    """placeholders for future updates or padding"""
    reserved1::Int16
    reserved2::Int16
    reserved3::Int16

    """Year in which this coherent integration block was captured"""
    yearCaptured::Int16
    """Month in which this coherent integration block was captured"""
    monthCaptured::Int16
    """Day in which this coherent integration block was captured"""
    dayCaptured::Int16
    """Hour in which first pulse in this specific coherent integration block was captured"""
    hourCaptured::Int16
    """Min in which first pulse in this specific coherent integration block was captured"""
    minCaptured::Int16
    """Second in which first pulse in this specific coherent integration block was captured"""
    secCaptured::Int16

    """placeholders for future updates or padding"""
    reserved4::Int16

    """Geographic latitude"""
    latitude::Float32
    """Geographic Longitude"""
    longitude::Float32
    """Geographic Altitude"""
    altitude::Float32

    """Total transmit power."""
    totalPowerRadiated::Float32

    """
    The ACARR radar array is divided into sub-arrays or "clusters" of antennas. This
    array tracks the power output of each specific hardware cluster for diagnostics and
    beam-forming calibration.
    """
    clusterWiseRadiatedPower::Vector{Float32} # 13 elements

    """
    Identifier for the operational mode (e.g., DBS wind profiling, spaced-antenna
    drift, meteor tracking).
    """
    operationMode::Int16

    """
    Tracks the current scan beam cycle
    ST radars typically operate in DBS (Doppler Beam Swinging) mode, cycling through
    several beam directions (e.g., Zenith, North, East, South, West) to resolve the 3D
    wind vector.
    """
    currentBeamCount::Int16
    """
    Number of beams
    ST radars typically operate in DBS (Doppler Beam Swinging) mode, cycling through
    several beam directions (e.g., Zenith, North, East, South, West) to resolve the 3D
    wind vector.
    """
    beamCount::Int16

    """
    number of bits in the phase code. Unsigned 16-bit code.
    """
    codeLength::Int16

    """ASCII strings for operator notes and experiment naming."""
    comment::Vector{Int8} # 16 elements

    """
    Transmit/Receive Path Delay in microseconds. The internal hardware time it takes for the
    signal to travel through cables, T/R switches, and filters. This must be subtracted from the
    time-of-flight to accurately calculate "Altitude Zero".
    """
    TRP_RF_Delay_us::Float32

    """
    Manual Gain Control in dB. The analog amplification applied to the receiver chain before
    ADC conversion to prevent saturation from ground clutter.
    """
    MGC_Gain_dB::Float32

    """
    The mathematical window function (e.g., Hanning, Hamming, Blackman) applied to the
    time-domain data before the FFT to reduce spectral leakage.
    """
    windowType::Int16

    """placeholders for future updates or padding"""
    reserved5::Int16

    """ASCII strings for operator notes and experiment naming."""
    remarks::Vector{Int8} # 512 elements

    """
    The actual binary sequences used for phase modulation.
    Radars often use complementary code pairs (Code A and Code B)
    transmitted on alternating pulses to cancel out range sidelobes perfectly.
    """
    PMCodeA::Vector{UInt32} # 2 elements
    """
    The actual binary sequences used for phase modulation.
    Radars often use complementary code pairs (Code A and Code B)
    transmitted on alternating pulses to cancel out range sidelobes perfectly.
    """
    PMCodeB::Vector{UInt32} # 2 elements

    """
    Flag distinguishing between actual atmospheric measurement runs and internal
    hardware calibration/noise-floor sampling.
    """
    experimentDataEnabled::Int8

    """placeholders for future software updates."""
    reservedCharArr::Vector{Int8} # 3 elements

    """The azimuthal pointing direction of the current beam."""
    azimuth::Float32
    """
    The off-zeneith pointing direction of the current beam. Off-Zenith is the elevation angle
    relative to straight up (usually around 10° to 15° for ST atmospheric profiling).
    """
    offZenith::Float32

    """Start of altitude boundaries (in km or meters) of 1st observation windows"""
    window1StartHeight::Float32
    """End of altitude boundaries (in km or meters) of 1st observation windows"""
    window1EndHeight::Float32

    """Start of altitude boundaries (in km or meters) of 2st observation windows"""
    window2StartHeight::Float32
    """End of altitude boundaries (in km or meters) of 2st observation windows"""
    window2EndHeight::Float32

    """Start of altitude boundaries (in km or meters) of 3st observation windows"""
    window3StartHeight::Float32
    """End of altitude boundaries (in km or meters) of 3st observation windows"""
    window3EndHeight::Float32

    """Start of altitude boundaries (in km or meters) of 4st observation windows"""
    window4StartHeight::Float32
    """End of altitude boundaries (in km or meters) of 4st observation windows"""
    window4EndHeight::Float32

    """Start of altitude boundaries (in km or meters) of 5st observation windows"""
    window5StartHeight::Float32
    """End of altitude boundaries (in km or meters) of 5st observation windows"""
    window5EndHeight::Float32
end

"""
Extended structure to hold both header and the 3D data block
"""
struct BeamObject
    header::RawBeamHeader
    """
    Beam Data
    Dimensions: [inCohIntegrations][rangeBins][FFTPoints]
    """
    BeamData::Array{ComplexF32, 3}
    
    """
    Resolution of each height. The radar pulse has to travel to the target and bounce back, this
    creates the minimum resolvable range. Range Resolution (ΔR) is: c * pulseDuration / 2
    """
    rangeResolution::Float32

    """
    Maximum Unambiguous Doppler Velocity / Nyquist velocity. If the wind blows faster than this
    limit, the signal aliases over to the opposite side of the spectrum
    It is given by: wavelength / (4 Teff), where Teff is effective time between each samples:
    Teff =  IPP * numOfCohIntegrations
    """
    maxVelocity::Float32

    """
    Directinon in which beam is aligned
    """
    direction::String
end

const DEFAULT_MAGIC_NUMBER = Int16(369)
const BEAM_HEADER_SIZE = 732

function read_RawBeamHeader(io::IO)::RawBeamHeader
    fileMagicNumber = read(io, Int16)
    rangeBins = read(io, Int16)
    baudLength_us = read(io, Float32)
    FFTPoints = read(io, Int16)
    numOfCohIntegrations = read(io, Int16)
    inCohIntegrations = read(io, Int16)
    codeFlag = read(io, Int16)
    IIP_us = read(io, Float32)
    pulseWidth_us = read(io, Float32)
    numOfObservedWindows = read(io, Int16)
    stationID = read(io, Int16)
    reserved1 = read(io, Int16)
    reserved2 = read(io, Int16)
    reserved3 = read(io, Int16)
    yearCaptured = read(io, Int16)
    monthCaptured = read(io, Int16)
    dayCaptured = read(io, Int16)
    hourCaptured = read(io, Int16)
    minCaptured = read(io, Int16)
    secCaptured = read(io, Int16)
    reserved4 = read(io, Int16)
    latitude = read(io, Float32)
    longitude = read(io, Float32)
    altitude = read(io, Float32)
    totalPowerRadiated = read(io, Float32)
    
    clusterWiseRadiatedPower = zeros(Float32, 13)
    readbytes!(io, reinterpret(UInt8, clusterWiseRadiatedPower), 13 * sizeof(Float32))
    
    operationMode = read(io, Int16)
    currentBeamCount = read(io, Int16)
    beamCount = read(io, Int16)
    codeLength = read(io, Int16)
    
    comment = zeros(Int8, 16)
    readbytes!(io, reinterpret(UInt8, comment), 16)
    
    TRP_RF_Delay_us = read(io, Float32)
    MGC_Gain_dB = read(io, Float32)
    windowType = read(io, Int16)
    reserved5 = read(io, Int16)
    
    remarks = zeros(Int8, 512)
    readbytes!(io, reinterpret(UInt8, remarks), 512)
    
    PMCodeA = zeros(UInt32, 2)
    readbytes!(io, reinterpret(UInt8, PMCodeA), 2 * sizeof(UInt32))
    PMCodeB = zeros(UInt32, 2)
    readbytes!(io, reinterpret(UInt8, PMCodeB), 2 * sizeof(UInt32))
    
    experimentDataEnabled = read(io, Int8)
    
    reservedCharArr = zeros(Int8, 3)
    readbytes!(io, reinterpret(UInt8, reservedCharArr), 3)
    
    azimuth = read(io, Float32)
    offZenith = read(io, Float32)
    window1StartHeight = read(io, Float32)
    window1EndHeight = read(io, Float32)
    window2StartHeight = read(io, Float32)
    window2EndHeight = read(io, Float32)
    window3StartHeight = read(io, Float32)
    window3EndHeight = read(io, Float32)
    window4StartHeight = read(io, Float32)
    window4EndHeight = read(io, Float32)
    window5StartHeight = read(io, Float32)
    window5EndHeight = read(io, Float32)

    return RawBeamHeader(
        fileMagicNumber, rangeBins, baudLength_us, FFTPoints, numOfCohIntegrations,
        inCohIntegrations, codeFlag, IIP_us, pulseWidth_us, numOfObservedWindows,
        stationID, reserved1, reserved2, reserved3, yearCaptured, monthCaptured,
        dayCaptured, hourCaptured, minCaptured, secCaptured, reserved4, latitude,
        longitude, altitude, totalPowerRadiated, clusterWiseRadiatedPower, operationMode,
        currentBeamCount, beamCount, codeLength, comment, TRP_RF_Delay_us, MGC_Gain_dB,
        windowType, reserved5, remarks, PMCodeA, PMCodeB, experimentDataEnabled,
        reservedCharArr, azimuth, offZenith, window1StartHeight, window1EndHeight,
        window2StartHeight, window2EndHeight, window3StartHeight, window3EndHeight,
        window4StartHeight, window4EndHeight, window5StartHeight, window5EndHeight
    )
end

function getRangeResolution(beamHeader::RawBeamHeader)::Float32
    c = 299792458f0
    # Check if phase-coding (pulse compression) is enabled
    # Usually, 1 or >0 means enabled, 0 means disabled
    if beamHeader.codeFlag > 0
        # Use Baud Length (the width of the compressed "chip")
        effective_time_us = beamHeader.baudLength_us
    else
        # Use the full uncompressed Pulse Width
        effective_time_us = beamHeader.pulseWidth_us
    end

    # Calculate Delta R
    return (c * effective_time_us * 1f-6) / 2f0
end

"""
Calculates the Nyquist velocity (maximum unambiguous velocity) in m/s. Defaults to 53 MHz
(Standard VHF ST Radar frequency).
"""
function getMaxVelocity(beamHeader::RawBeamHeader, radarFreq::Float32 = 205f6)::Float32
    c = 299792458f0
    wavelength = c / radarFreq
    # 2. Calculate the effective time between FFT samples(in seconds)
    effective_sampling_time = beamHeader.IIP_us * 1f-6 * beamHeader.numOfCohIntegrations
    # 3. Calculate Vmax
    return wavelength / (4f0 * effective_sampling_time)
end

function getBeamDirection(azimuth::Float32, offZenith::Float32)::String
    if offZenith == 0f0 
        return "Vertical"
    end
    azimuth_int = round(Int, azimuth)
    if azimuth_int == 0
        return "North"
    elseif azimuth_int == 90
        return "West"
    elseif azimuth_int == 180
        return "South"
    elseif azimuth_int == 270
        return "East"
    end
    return "Unknown"
end

"""
Reads given file path and returns BeamObject for it
"""
function read_data(filepath::String)::Vector{BeamObject}
    headers = Vector{BeamObject}()
    
    open(filepath, "r") do file
        beam_count = 1
        beam_No = 0
        
        while beam_No < beam_count
            header = read_RawBeamHeader(file)
            
            if header.fileMagicNumber != DEFAULT_MAGIC_NUMBER
                error("Wrong File Selected: Invalid Magic Number.")
            end
            
            # MATLAB reads up to 1024 bytes for the full header block. We jump the remainder.
            skip(file, 1024 - BEAM_HEADER_SIZE)
            
            # Update beam_count dynamically from the first valid header read
            beam_count = header.beamCount
            
            inCohIntegrations = Int(header.inCohIntegrations)
            if inCohIntegrations <= 0
                inCohIntegrations = 1
            end
            
            nBins = Int(header.rangeBins)
            nFFT = Int(header.FFTPoints)
            dataBlockSize = nBins * nFFT
            
            # Allocate 3D Data structure: [InCohIntegration, RangeBin, NFFT]
            BeamData = Array{ComplexF32, 3}(undef, inCohIntegrations, nBins, nFFT)
            
            for InCohInt in 1:inCohIntegrations
                if InCohInt != 1
                    # Subsequent integrations are prefaced with a dummy/secondary 1024-byte header
                    dummyHeader = zeros(UInt8, 1024)
                    bytes_read = readbytes!(file, dummyHeader, 1024)
                    if bytes_read != 1024
                        error("Error in file Read: Expected Header Data Count 1024")
                    end
                end
                
                # Read the block of complex integers
                # C++ std::complex<int32_t> is layout-compatible with int32_t[2]
                rawData = zeros(Int32, 2 * dataBlockSize)
                bytes_to_read = dataBlockSize * 2 * sizeof(Int32)
                bytes_read = readbytes!(file, reinterpret(UInt8, rawData), bytes_to_read)
                
                if bytes_read != bytes_to_read
                    error("Error in file Read: Expected Data Count mismatch.")
                end
                
                # Map 1D flat interleaved array into 2D (Range x FFT) matrix
                # Equivalent to MATLAB's transpose and reshape operations
                # The data is stored such that rawData[fft * nBins + r] has real and imag parts.
                # In rawData, index is 1-based.
                for fft in 1:nFFT
                    for r in 1:nBins
                        idx = (fft - 1) * nBins + r
                        real_part = rawData[2 * idx - 1]
                        imag_part = rawData[2 * idx]
                        
                        # Note: We cast to Float32 here, mimicking Python's complex128/64, 
                        # and C++ std::conj(rawData[...])
                        BeamData[InCohInt, r, fft] = ComplexF32(real_part, -imag_part)
                    end
                end
            end
            
            rangeRes = getRangeResolution(header)
            maxVel = getMaxVelocity(header) # Note: C++ had a bug calling getRangeResolution instead of getMaxVelocity for maxVelocity! Let's fix it or keep the bug? C++ read: beamObj.maxVelocity = getRangeResolution(beamObj.header); but python uses it as maxVelocity. Let's fix it by using getMaxVelocity properly.
            direction = getBeamDirection(header.azimuth, header.offZenith)
            
            push!(headers, BeamObject(header, BeamData, rangeRes, maxVel, direction))
            
            beam_No += 1
        end
    end
    
    return headers
end

end # module
