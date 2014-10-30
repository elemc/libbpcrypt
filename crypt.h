/*
   crypt.h:
   routines in libbpcrypt.a
   and libbpcrypt.so
*/

#ifndef CRYPT_H
#define CRYPT_H

#include "misc.h"

#ifdef __cplusplus
extern "C"{
#endif 

extern bp_buffer_t *encrypt_buffer( bp_buffer_t *buffer, bp_size_t buffer_size, 
                                    bp_buffer_t *key, bp_size_t key_size,
                                    bp_size_t *cipher_buffer_size);
extern bp_buffer_t *decrypt_buffer( bp_buffer_t *buffer, bp_size_t buffer_size,
                                    bp_buffer_t *key, bp_size_t key_size,
                                    bp_size_t *clean_buffer_size );

#ifdef __cplusplus
}
#endif

#endif
