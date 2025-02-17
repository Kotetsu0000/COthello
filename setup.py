from setuptools import setup, find_packages
from pybind11.setup_helpers import Pybind11Extension, build_ext

__version__ = "0.0.2"

ext_modules = [
    Pybind11Extension(
        "COthello._COthello",
        ["src/COthello.cpp", "src/Board.cpp", "src/Perft.cpp"],
        include_dirs=["include"],
        extra_compile_args=['-std=c++20', '-O3', '-march=native', '-fopenmp'],
        extra_link_args=['-fopenmp'],
    ),
]

setup(
    name="COthello",
    version=__version__,
    author="Kotetsu0000",
    author_email="kotech.dev@gmail.com",
    description="A fast Othello library using SIMD",
    long_description="COthello is a fast Othello library using SIMD.",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    packages=find_packages(),
    zip_safe=False,
    python_requires=">=3.7",
)