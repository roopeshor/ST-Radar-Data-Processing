#!/bin/bash
cd "reader" || exit

python extract_docs.py
python setup.py build_ext --inplace

read -p "Install type hintes [y/n]?" yn
case $yn in
    Y | y ) pip install pybind11-stubgen; pybind11-stubgen st_radar -o . ;;
    * ) echo "Not creating type hints" ;;
esac