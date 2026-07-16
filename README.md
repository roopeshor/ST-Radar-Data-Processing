# ST radar
Reads ST Radar raw files and displays wind speeds in 5 directions (NEWS+Z).
Implemented in Python+Cpp, Julia and MATLAB.
While running, each scripts assume that a `Data` folder is sitting just next to it:
```
├── run.[m/py/jl]
├── Data
│   ├── Mode_1
│   │   ├── *.raw
```

## Running Julia:
```bash 
cd julia
julia --project=. Run.jl
```

## Running Matlab
run `matlab/run.m`

## Setup for Python+Cpp
### With bash:
```bash
cd python_cpp
chmod +x setup.sh
./setup.sh
```

### Manually:

```sh
cd python_cpp
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
See `python_cpp/run.py`