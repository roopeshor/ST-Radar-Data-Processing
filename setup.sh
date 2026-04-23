#!/bin/bash
cd "reader" || exit
rm "build" ./*.so st_radar.pyi auto_docs.hpp -r
python extract_docs.py
python setup.py build_ext --inplace

read -p "Install type hintes [y/n]?" yn
case $yn in
    Y | y | "" )
        if python3 -c "import pybind11_stubgen" &> /dev/null; then
            export PYTHONPATH="./:$PYTHONPATH"
            pybind11-stubgen st_radar -o .
            echo "Created Stub"
        else
            echo "Module 'pybind11_stubgen' is missing. Install it"
        fi
        ;;
    * ) echo "Not creating type hints" ;;
esac