from reader import st_radar
import numpy as np
from proc import *
from pathlib import Path

folder = "Data/Mode_1"
raws = list(Path(folder).glob("*.raw"))

# incoherently integrated spctrum
spectrums: list[PowerSpectrum] = list()
dat = st_radar.read_data(raws[3]._str)
# raws = []
# for file in raws:
#     filepath = file._str
#     print(filepath)
#     beams = st_radar.read_data(filepath)

#     beam_0 = beams[0]

#     # Access scalar properties
#     print(f"Total Beams: {beam_0.header.beamCount}")
#     print(f"Azimuth: {beam_0.header.azimuth}")

#     print(f"Comments: {beam_0.header.comment}")
#     print(f"Cluster Power: {beam_0.header.clusterWiseRadiatedPower}")

#     # Convert the nested std::vector into a dense NumPy array for processing
#     # Shape will be: (InCohIntegrations, RangeBins, FFTPoints)

#     data_matrix = np.array(beam_0.BeamData)
#     print(f"Data Matrix Shape: {data_matrix.shape}")
#     print(f"Data Type: {data_matrix.dtype}") # Will output complex128 or complex64

#     spectrums.append(process_radar_data(data_matrix))

# visualize_spectra(spectrums[0])
beams = [
    dat[1],
    dat[2],
    dat[0],
    dat[3],
    dat[4],
]
visualize_spectra(beams)