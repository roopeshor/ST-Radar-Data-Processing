from reader import st_radar
import numpy as np

filepath = "./Data/Mode_2/EXP_DBS_CH4_01Jun2025_23_41_50.raw"

beams = st_radar.read_data(filepath)

beam_0 = beams[0]

# Access scalar properties
print(f"Total Beams: {beam_0.cfg.m_sTotalNumberofBeams}")
print(f"Azimuth: {beam_0.cfg.m_fAzimuth}")

# Access array properties (evaluated via C++ lambdas)
print(f"Comments: {beam_0.cfg.m_cComment}")
print(f"Cluster Power: {beam_0.cfg.m_fRadiatedPower_ClusterWise}")

# Convert the nested std::vector into a dense NumPy array for processing
# Shape will be: (InCohIntegrations, RangeBins, NFFT)
data_matrix = np.array(beam_0.BeamData)
print(f"Data Matrix Shape: {data_matrix.shape}")
print(f"Data Type: {data_matrix.dtype}") # Will output complex128 or complex64