# ST radar
Reads ST Radar raw files and displays wind speeds in 5 directions (NEWS+Z).
Implemented in Python+Cpp, Julia and MATLAB

## Running Julia:
```bash 
cd julia
julia --project=. Run.jl
```

## Running Matlab
run `run_script.m`

## Setup for Python+Cpp
### With bash:
```bash 
chmod +x setup.sh
./setup.sh
```

### Manually:

```sh
cd "reader"
python extract_docs.py
python setup.py build_ext --inplace
```

if you want to create type hints (intellisense hints):
```bash
pip install pybind11 pybind11-stubgen
pybind11-stubgen st_radar -o .
```

### Usage
See `run.py`