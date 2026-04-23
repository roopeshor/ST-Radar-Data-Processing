from reader import st_radar
import numpy as np
from proc import process_radar_data, visualize_spectra
filepath = "./Data/Mode_2/EXP_DBS_CH4_01Jun2025_18_04_28.raw"

beams = st_radar.read_data(filepath)

beam_0 = beams[0]

# Access scalar properties
print(f"Total Beams: {beam_0.header.beamCount}")
print(f"Azimuth: {beam_0.header.azimuth}")

print(f"Comments: {beam_0.header.comment}")
print(f"Cluster Power: {beam_0.header.clusterWiseRadiatedPower}")

# Convert the nested std::vector into a dense NumPy array for processing
# Shape will be: (InCohIntegrations, RangeBins, FFTPoints)

data_matrix = np.array(beam_0.BeamData)
print(f"Data Matrix Shape: {data_matrix.shape}")
print(f"Data Type: {data_matrix.dtype}") # Will output complex128 or complex64

visualize_spectra(process_radar_data(data_matrix))