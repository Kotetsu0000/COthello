import setuptools
from pybind11.setup_helpers import Pybind11Extension, build_ext

__version__ = "0.0.1"

ext_modules = [
    Pybind11Extension(
        "COthello",
        ["src/COthello.cpp"],
        include_dirs=["include"],
    ),
]

setuptools.setup(
    name="COthello",
    version=__version__,
    author="Kotetsu0000",
    author_email="kotech. পাঠানোর000@gmail.com",
    description="A fast Othello library using SIMD",
    long_description="COthello is a fast Othello library using SIMD.",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    python_requires=">=3.7",
    install_requires=['pybind11', 'numpy'],
)