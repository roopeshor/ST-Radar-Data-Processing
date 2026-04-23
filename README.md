# ST radar
Reads ST Radar raw files and converts it to Numpy object
## Setup
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
pip install pybind11-stubgen;
pybind11-stubgen st_radar -o .
```

## Usage
See `run.py`