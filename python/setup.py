#!/usr/bin/env python
# -*- coding: utf-8 -*-
# -*- Python -*-
# ------------------------------------ #
# Python source single (setup.py)      #
# Author: Alexei Panov <me@elemc.name> #
# ------------------------------------ #
# Description: 

from distutils.core import setup, Extension
setup(
    name='bpcrypt',
    version='0.1.0',
    ext_modules = [
        Extension(
            'bpcrypt',
            ['bpcryptmodule.c'],
            include_dirs=['..'],
            libraries=['bpcrypt'],
            library_dirs=['../build/']
        )
    ],
    author="Alexei Panov",
    author_email="me@elemc.name",
    url="https://bitbucket.org/elemc/libbpcrypt",
)


