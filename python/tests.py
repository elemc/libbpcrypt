#!/usr/bin/env python
# -*- coding: utf-8 -*-
# -*- Python -*-
# ------------------------------------ #
# Python source single (tests.py)      #
# Author: Alexei Panov <me@elemc.name> #
# ------------------------------------ #
# Description:                         # 
#    tests for bpcrypt python module   #


import sys, os
import bpcrypt

if __name__ == '__main__':
    buf = "Я помню чудное мгновенье, передо мной явилась ты...";
    key = "Паролик";

    crypt_buf = bpcrypt.encrypt_buffer( buf, key );
    clean_buf = bpcrypt.decrypt_buffer( crypt_buf, key );

    if ( clean_buf == buf ):
        print("bpcrypt.encrypt_buffer test pass. OK.")
        print("bpcrypt.decrypt_buffer test pass. OK.")
        os._exit( 0 )
