import os
import sys
import numpy as np
from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import pybind11

# Get numpy include directory
numpy_include = np.get_include()

# Platform-specific compiler settings
if sys.platform == 'win32':
    extra_compile_args = ['/std:c++17']
    extra_link_args = []
else:
    # For macOS and Linux
    extra_compile_args = ['-std=c++17', '-O3', '-march=native']
    extra_link_args = []

# Define the extension module
ext_modules = [
    Extension(
        'pyyawt._pyyawt',
        sources=['pyyawt/wavelet.cpp'],
        include_dirs=[
            numpy_include,
            pybind11.get_include(),
            pybind11.get_include(user=True)
        ],
        language='c++',
        extra_compile_args=extra_compile_args,
        extra_link_args=extra_link_args,
    )
]

setup(
    name='pyyawt',
    version='0.1.0',
    description='C++ implementation of wavelet transforms',
    author='rsHRF Team',
    author_email='madhurtandon23@gmail.com',
    packages=['pyyawt'],
    ext_modules=ext_modules,
    python_requires='>=3.6',
    install_requires=[
        'numpy',
        'pybind11>=2.6.0'
    ],
) 