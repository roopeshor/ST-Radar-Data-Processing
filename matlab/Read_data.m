
% **************************************************************************
%  File Name	: RawDataRead.m
%  Author		: Linto Thomas
%  Created Date	: 30 SEP 2016
%  Description	: This file is used to read ST Radar raw Data header and Data.
%                 this script will read first header followed by first beam
%                 Data. use the same script in loop till end of file for
%                 entire data read 
% **************************************************************************
% **************************************************************************
%  Company Name: Indus Teqsite Pvt. Ltd. 
%  Address: Plot # H9, Fourth Main Road, SIPCOT IT Park, Siruseri, 
% 			Off Rajiv Gandhi Salai (OMR), Pudupakkam P.O., 
% 			Chennai-603103 | India
%  Email: 
%  Phone: 91-44-4741-4000
%  FAX: 91-44-4741-4444
% **************************************************************************
%clear all;


function Header = Read_data(filepath)
    FileName=filepath;
    size_of_file=dir(FileName);
    fPtr = fopen(FileName, 'rb');
    
    DEFAULT_MAGIC_NUMBER = 369; % 
    
    size_BeamConfig = 732; %bytes
    
    %--------------Find the no of beams---------------
    ss=fread(fPtr);
    beam_count=ss(121,1);
    %------------5--Find the no of beams---------------
    c=1;
    fPtr = fopen(FileName, 'rb');
    
    for beam_No= 1:beam_count
    %if File read reached end of the file , Move file pointer to beginig of the
    %if(fPtr~=-1)
        Header(beam_No).m_sMagicNumber= fread(fPtr,1,'int16');
        
        if(Header(beam_No).m_sMagicNumber ~= DEFAULT_MAGIC_NUMBER)
            WarnDlgHandle = warndlg('Wrong File Selected','ST Radar Warning','modal');waitfor(WarnDlgHandle);
            fclose(fPtr);
            set(handles.FilePath,'String',''); %Clear Edit Box
        else
            Header(beam_No).m_sNumOfRangeBins = fread(fPtr,1,'int16');
     
            Header(beam_No).m_fBaudLength_us = fread(fPtr,1,'single');
            
            Header(beam_No).m_sNFFT = fread(fPtr,1,'int16');
            g_NFFT =  Header(beam_No).m_sNFFT;
            
            Header(beam_No).m_sNumOfCohIntegrations = fread(fPtr,1,'int16');
            Header(beam_No).m_sNumOfInCohIntegrations = fread(fPtr,1,'int16');
            Header(beam_No).m_sCodeFlag = fread(fPtr,1,'int16');
            
            Header(beam_No).m_fIntrPulsePeriod_us = fread(fPtr,1,'single');
            Header(beam_No).m_fPulseWidth_us = fread(fPtr,1,'single');
            
            Header(beam_No).m_sNumOfObservWindows = fread(fPtr,1,'int16');
            Header(beam_No).m_sStationID = fread(fPtr,1,'int16');
            Header(beam_No).m_sReserved1 = fread(fPtr,1,'int16');
            Header(beam_No).m_sReserved2 = fread(fPtr,1,'int16');
            
            Header(beam_No).m_sReserved3 = fread(fPtr,1,'int16');
            Header(beam_No).m_sYear = fread(fPtr,1,'int16');
            Header(beam_No).m_sMonth = fread(fPtr,1,'int16');
            Header(beam_No).m_sDay = fread(fPtr,1,'int16');
            
            Header(beam_No).m_sHour = fread(fPtr,1,'int16');
            Header(beam_No).m_sMin = fread(fPtr,1,'int16');
            Header(beam_No).m_sSec = fread(fPtr,1,'int16');
            Header(beam_No).m_sReserved4 = fread(fPtr,1,'int16');
            
            Header(beam_No).m_fLatitude = fread(fPtr,1,'single');
            Header(beam_No).m_fLongitude = fread(fPtr,1,'single');
            Header(beam_No).m_fAltitude = fread(fPtr,1,'single');
            Header(beam_No).m_fTotalPowerRadiated = fread(fPtr,1,'single');
            Header(beam_No).m_fRadiatedPower_ClusterWise= fread(fPtr,13,'single');
            Header(beam_No).m_sOperationMode = fread(fPtr,1,'int16');
            Header(beam_No).m_sCurrentBeamCnt = fread(fPtr,1,'int16');
            Header(beam_No).m_sTotalNumberofBeams = fread(fPtr,1,'int16');
            Header(beam_No).m_usCodeLength = fread(fPtr,1,'int16');
            
            Header(beam_No).m_cComment = fread(fPtr,16,'int8');
            
            Header(beam_No).m_fTRP_RF_Delay_us = fread(fPtr,1,'single');
            Header(beam_No).m_fMGC_Gain_dB=  fread(fPtr,1,'single');
            Header(beam_No).m_usWindowType = fread(fPtr,1,'int16');
            Header(beam_No).m_usReserved = fread(fPtr,1,'int16');
            Header(beam_No).m_cArrRemarks = fread(fPtr,512,'int8');
            Header(beam_No).m_ulCodeA = fread(fPtr,2,'uint32');
            Header(beam_No).m_ulCodeB = fread(fPtr,2,'uint32');
            
            Header(beam_No).m_ucExperimentDataEnable=  fread(fPtr,1,'int8');
            Header(beam_No).m_cReserved =  fread(fPtr,3,'int8');
            Header(beam_No).m_fAzimuth =  fread(fPtr,1,'single');
            Header(beam_No).m_fOffZenith =  fread(fPtr,1,'single');
            
            
            Header(beam_No).m_fWindow1StartHeight = fread(fPtr,1,'single');
            Header(beam_No).m_fWindow1EndHeight = fread(fPtr,1,'single');
            Header(beam_No).m_fWindow2StartHeight = fread(fPtr,1,'single');
            Header(beam_No).m_fWindow2EndHeight = fread(fPtr,1,'single');
            Header(beam_No).m_fWindow3StartHeight = fread(fPtr,1,'single');
            Header(beam_No).m_fWindow3EndHeight = fread(fPtr,1,'single');
            Header(beam_No).m_fWindow4StartHeight = fread(fPtr,1,'single');
            Header(beam_No).m_fWindow4EndHeight = fread(fPtr,1,'single');
            Header(beam_No).m_fWindow5StartHeight = fread(fPtr,1,'single');
            Header(beam_No).m_fWindow5EndHeight = fread(fPtr,1,'single');
            
            ReservedData = fread(fPtr,(1024 - size_BeamConfig),'int8');
            beam_count=Header.m_sTotalNumberofBeams;
            
            
            %In DMA Data processing NSA =0 (non experiment condition)
            if(Header(beam_No).m_sNumOfInCohIntegrations <= 0)
                Header(beam_No).m_sNumOfInCohIntegrations =1;
            end
            
            for InCohIntegration = 1 : Header(beam_No).m_sNumOfInCohIntegrations
                
                if(InCohIntegration ~= 1)
                    [FileHeader, HeaderDataCount] = fread(fPtr, 1024, 'int8');
                    if(HeaderDataCount ~= 1024)
                        errordlg(sprintf('Error in file Read \nExpected Header Data Count %d \nRead Data Count %d ',1024, HeaderDataCount));
                    end
                else
                    BeamData = [];
                end
                
                DataBlockSize =  Header(beam_No).m_sNumOfRangeBins* Header(beam_No).m_sNFFT ;
                DataSize = ftell(fPtr);
                RawData =  fread(fPtr, [2, DataBlockSize], 'int32')';
                
                          
                if((ftell(fPtr) - DataSize)~=(2*DataBlockSize * 4))
                    errordlg(sprintf('Error in file Read \nExpected Data Count %d \nRead Data Count %d ',(2*DataBlockSize)));
                end
                
                in_data = complex(RawData(:,1), RawData(:,2));
                
                g_OfflinePowerSpectradata = zeros(Header(beam_No).m_sNFFT, Header(beam_No).m_sNumOfRangeBins);
                
                %Form a matrix of Range x FFT Size data
                for LoopCounter = 0:(Header(beam_No).m_sNFFT-1)
                    g_OfflinePowerSpectradata(LoopCounter+1, :) = in_data(((LoopCounter*Header(beam_No).m_sNumOfRangeBins) + 1) : ((LoopCounter * Header(beam_No).m_sNumOfRangeBins) + Header(beam_No).m_sNumOfRangeBins));
                end
                
                Header(beam_No).BeamData(:,:,InCohIntegration) = g_OfflinePowerSpectradata';
                
            end
        end
    end
fclose(fPtr);    
end
