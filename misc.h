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

#ifdef __cplusplus
extern "C"{
#endif 

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <openssl/md5.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "config.h"

#if !defined(_WIN32) && !defined(_WIN64)
#include <err.h>
extern int errno;
#endif

#define BP_READ_BLOCK_SIZE 2048

typedef unsigned char bp_buffer_t;
typedef int32_t bp_size_t;

extern bp_buffer_t *BP_md5_hash( bp_buffer_t *buffer, bp_size_t buffer_size );
extern bp_buffer_t *BP_to_hex( bp_buffer_t *buffer, bp_size_t buffer_size, bp_size_t *hex_size);
extern bp_buffer_t *BP_read_file( const char *filename, bp_size_t *file_size );
extern bp_buffer_t *BP_copy_ptr( bp_buffer_t *ptr );
extern bp_buffer_t *BP_ncopy_ptr( bp_buffer_t *ptr, bp_size_t size );
extern void BP_error( int code, int eval, const char *fmt, ... );

#ifdef __cplusplus
}
#endif

#endif

