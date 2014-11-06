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

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <openssl/md5.h>
#include <error.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include "config.h"

#ifdef __cplusplus
extern "C"{
#endif 

extern int errno;

#define BP_READ_BLOCK_SIZE 2048

typedef unsigned char bp_buffer_t;
typedef int32_t bp_size_t;

extern bp_buffer_t *BP_md5_hash( bp_buffer_t *buffer, bp_size_t buffer_size );
extern bp_buffer_t *BP_to_hex( bp_buffer_t *buffer, bp_size_t buffer_size, bp_size_t *hex_size);
extern bp_buffer_t *BP_read_file( bp_buffer_t *filename, bp_size_t filename_size, bp_size_t *file_size );

#ifdef __cplusplus
}
#endif

#endif

