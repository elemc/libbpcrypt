// -*- C -*-
/* ------------------------------------ */
/* C source header                      */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */

/* Name: (old_crypt)
   Description: 
*/

#ifndef OLD_CRYPT_H
#define OLD_CRYPT_H

#include "crypt.h"

#ifdef __cplusplus
extern "C"{
#endif 

extern bp_buffer_t *old_encrypt_buffer( bp_buffer_t *buffer, bp_size_t buffer_size, 
                                        bp_buffer_t *key, bp_size_t key_size,
                                        bp_size_t *cipher_buffer_size);
extern bp_buffer_t *old_decrypt_buffer( bp_buffer_t *buffer, bp_size_t buffer_size,
                                        bp_buffer_t *key, bp_size_t key_size,
                                        bp_size_t *clean_buffer_size );

#ifdef __cplusplus
}
#endif



#endif

