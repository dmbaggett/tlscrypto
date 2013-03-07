#!/usr/bin/env python
#
# Setup for tlscrypto, a minimal subset of pycrypto 1.6 for use with tlslite.
#
# March 5, 2013 - Dave Baggett (Arcode Corporation)
#

from distutils.core import setup, Extension

extra_compile_args = ['-Iinclude']
extra_link_args = []

setup(name='tlscrypto',
      version='1.0',
      description='Minimal subset of pycrypto 1.6 for use with tlslite',
      author='Dave Baggett',
      author_email='dmb@inky.com',
      url='https://github.com/dmbaggett/tlscrypto',
      packages=['tlscrypto'],
      ext_modules=[
        Extension(
            name='tlscrypto/_AES',
            sources=['tlscrypto/aes/AES.c'],
            extra_compile_args=extra_compile_args,
            extra_link_args=extra_link_args),
        Extension(
            name='tlscrypto/_isaac',
            sources=['tlscrypto/isaac/isaac.c'],
            extra_compile_args=extra_compile_args,
            extra_link_args=extra_link_args),
        Extension(
            name='tlscrypto/_MD2',
            sources=['tlscrypto/md2/MD2.c'],
            extra_compile_args=extra_compile_args,
            extra_link_args=extra_link_args),
        Extension(
            name='tlscrypto/_MD4',
            sources=['tlscrypto/md4/MD4.c'],
            extra_compile_args=extra_compile_args,
            extra_link_args=extra_link_args),
        Extension(
            name='tlscrypto/_ARC2',
            sources=['tlscrypto/rc2/ARC2.c'],
            extra_compile_args=extra_compile_args,
            extra_link_args=extra_link_args),
        Extension(
            name='tlscrypto/_ARC4',
            sources=['tlscrypto/rc4/ARC4.c'],
            extra_compile_args=extra_compile_args,
            extra_link_args=extra_link_args),
        ]
     )
