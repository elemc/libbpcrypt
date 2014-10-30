// -*- C -*-
/* ------------------------------------ */
/* C source header                      */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */

/* Name: (misc)
   Description: 
*/

#ifndef MISC_H
#define MISC_H

#include "crypt.h"
#include <openssl/md5.h>

#ifdef __cplusplus
extern "C"{
#endif 

extern bp_buffer_t *BP_md5_hash( bp_buffer_t *buffer, bp_size_t buffer_size );
extern bp_buffer_t *BP_to_hex( bp_buffer_t *buffer, bp_size_t buffer_size, bp_size_t *hex_size);

#ifdef __cplusplus
}
#endif

#endif

