#!/usr/bin/env python3
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
    buf = u'asd' #u"Я помню чудное мгновенье, передо мной явилась ты...";
    key = u'123' #u"Паролик";
    buf_data = buf.encode("UTF-8")
    key_data = buf.encode("UTF-8")

    crypt_buf = bpcrypt.encrypt_buffer( buf_data, key_data );
    clean_buf = bpcrypt.decrypt_buffer( crypt_buf, key_data );
    clean = clean_buf.decode("UTF-8")

    if ( clean == buf ):
        print("bpcrypt.encrypt_buffer test pass. OK.")
        print("bpcrypt.decrypt_buffer test pass. OK.")
        os._exit( 0 )
