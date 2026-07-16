import sys
import os
sys.path.append(os.path.abspath('..'))
from reader import st_radar
import numpy as np
from proc import process_radar_data
from pathlib import Path

def main():
    folder = "../Data/Mode_1"
    raws = list(Path(folder).glob("*.raw"))
    
    if not raws:
        print("No raw files found.")
        return
        
    filepath = raws[3]._str # python index 3 is julia index 4
    print(f"Reading {filepath} with Python...")
    dat = st_radar.read_data(filepath)
    
    beam_0 = dat[0]
    data_matrix = np.array(beam_0.BeamData)
    print(f"Data Matrix Shape: {data_matrix.shape}")
    
    # Save the input raw complex data as float32, since complex64 = 2x float32
    # C++ returns std::vector<std::complex<int32>> mapped to python complex128? Wait.
    # The pybind binding returns complex32 which doesn't exist natively in numpy, it gets casted to complex64 or complex128.
    print(f"Data Matrix Dtype: {data_matrix.dtype}")
    # Let's save the real and imaginary parts explicitly
    with open("py_raw_data_real.bin", "wb") as f:
        data_matrix.real.astype(np.float64).tofile(f)
    with open("py_raw_data_imag.bin", "wb") as f:
        data_matrix.imag.astype(np.float64).tofile(f)
    
    spectra, L = process_radar_data(data_matrix)
    
    print(f"Spectra shape: {spectra.shape}")
    print(f"L shape: {L.shape}")
    
    # Save output exactly
    with open("py_spectra.bin", "wb") as f:
        spectra.astype(np.float64).tofile(f)
        
    with open("py_L.bin", "wb") as f:
        L.astype(np.float64).tofile(f)
        
    print("Python outputs saved to binary files.")

if __name__ == "__main__":
    main()
