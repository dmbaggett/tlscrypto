#!/usr/bin/python
import sys
from distutils.core import setup, Extension

__version__ = "1.0.0"

extra_compile_args = ['-I..']
extra_link_args = []

setup(
    name="ISAAC",
    version=__version__,
    author="Bob Jenkins",
    url="http://www.burtleburtle.net/bob/rand/isaacafa.html",
    description="C implementation of ISAAC cryptographic PRNG, by Bob Jenkins.",
    license="N/A",
    platforms=["Platform Independent"],
    ext_modules=[Extension(
            name='_isaac',
            sources=['isaac.c', 'rand.c'],
            extra_compile_args=extra_compile_args,
            extra_link_args=extra_link_args)]
)
