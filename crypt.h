/*
   libbpcrypt.h:
   routines in libbpcrypt.a
   and libbpcrypt.so
*/
#include <stdint.h>

typedef unsigned char bp_buffer_t;
typedef int32_t bp_size_t;

extern bp_buffer_t *encrypt_buffer( bp_buffer_t *buffer, bp_size_t buffer_size, 
                                    bp_buffer_t *key, bp_size_t key_size,
                                    bp_size_t *cipher_buffer_size);
extern bp_buffer_t *decrypt_buffer( bp_buffer_t *buffer, bp_size_t buffer_size,
                                    bp_buffer_t *key, bp_size_t key_size,
                                    bp_size_t *clean_buffer_size );

