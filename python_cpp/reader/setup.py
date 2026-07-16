# setup.py
import sys
import subprocess
from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup

# 1. Run the extractor script before doing anything else
try:
    subprocess.check_call([sys.executable, "extract_docs.py"])
except subprocess.CalledProcessError:
    print("Warning: Failed to generate auto_docs.hpp. Make sure definitions.hpp exists.")

# 2. Proceed with standard compilation
ext_modules = [
    Pybind11Extension(
        "st_radar",
        ["binding.cpp", "reader.cpp"],
        cxx_std=11,
    ),
]

setup(
    name="st_radar",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
)