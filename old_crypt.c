// -*- C -*-
/* ------------------------------------ */
/* C source source                      */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */

/* Name: (old_crypt)
   Description: 
*/

#include "old_crypt.h"
#include <string.h>
#include <openssl/aes.h>

bp_buffer_t *hex_hash_key( bp_buffer_t *key, bp_size_t key_size ) {
    bp_buffer_t *hash_key = BP_md5_hash( key, key_size );
    bp_size_t hh_key_size;
    bp_buffer_t *hex_hash_key = BP_to_hex( hash_key, MD5_DIGEST_LENGTH, &hh_key_size );

    free( hash_key );
    return hex_hash_key;
}

bp_buffer_t *get_aes_key_from_password( bp_buffer_t *hex_hash_key ) {
    bp_buffer_t *b_key = calloc( sizeof( bp_buffer_t ), 17 );
    memcpy( (void *)b_key, hex_hash_key, 16 );

    bp_buffer_t *last = (b_key+16);
    *last = '\0';

    return b_key;
}

bp_buffer_t *get_aes_iv_from_password( bp_buffer_t *hex_hash_key ) {
    bp_buffer_t *b_iv = calloc( sizeof( bp_buffer_t ), 17 );
    memcpy( (void *)b_iv, hex_hash_key+16, 16 );

    bp_buffer_t *last = (b_iv+16);
    *last = '\0';

    return b_iv;
}

bp_buffer_t *openssl_crypt(bp_buffer_t *data, bp_size_t datalen, bp_buffer_t *key, bp_buffer_t *iv, int enc) {
    bp_buffer_t *temp_iv = calloc( sizeof( bp_buffer_t ), 16 );
    memcpy( temp_iv, iv, 16 );

    AES_KEY aes_key;
    if ( enc == AES_ENCRYPT )
        AES_set_encrypt_key((unsigned char*)key, 16*8, &aes_key);
    else
        AES_set_decrypt_key((unsigned char*)key, 16*8, &aes_key);

    bp_buffer_t *out_data = calloc( sizeof( bp_buffer_t ), datalen );
    if ( out_data == NULL ) {
        perror( "Data allocation (out_data) failed!" );
        return NULL;
    }

    AES_cbc_encrypt(data, out_data, datalen, &aes_key, temp_iv, enc);
    if ( out_data == NULL ) {
        perror( "Data encryption (out_data) failed!" );
        return NULL;
    }
    
    free( temp_iv );

    return out_data;
}

bp_buffer_t *old_encrypt_buffer( bp_buffer_t *buffer,    bp_size_t buffer_size,
                                 bp_buffer_t *key,       bp_size_t key_size,
                                 bp_size_t   *cipher_buffer_size ) {
    bp_buffer_t *hash_key   = hex_hash_key( key, key_size );
    bp_buffer_t *b_key      = get_aes_key_from_password( hash_key );
    bp_buffer_t *b_iv       = get_aes_iv_from_password( hash_key );

    bp_size_t temp_block_size = buffer_size % 16;
    bp_size_t last_block_size = temp_block_size == 0 ? 16 : temp_block_size;


    bp_buffer_t *service_block = calloc( sizeof( bp_buffer_t ), 16 );
    sprintf( (char *)service_block, "%d", last_block_size );

    bp_buffer_t *cipher_buffer;
    *cipher_buffer_size = 16;
    cipher_buffer = calloc( sizeof( bp_buffer_t ), *cipher_buffer_size );

    bp_buffer_t *cipher = openssl_crypt( service_block, 16, b_key, b_iv, AES_ENCRYPT );
    if ( cipher == NULL ) {
        perror( "Error in crypt service block." );
        return NULL;
    }
    memcpy( cipher_buffer, cipher, 16 );
    free( cipher );
    free( service_block );


    int i = 0;
    while( i < buffer_size ) {
        bp_buffer_t *temp_buf16 = calloc( sizeof( bp_buffer_t ), 16 );

        bp_buffer_t *seek_buffer = (buffer + i);
        if ( i + 16 > buffer_size ) {
            memcpy( temp_buf16, seek_buffer, buffer_size - i );
            temp_buf16[15] = buffer_size - i;
        }
        else
            memcpy( temp_buf16, seek_buffer, 16 );
        
        bp_buffer_t *cipher = openssl_crypt( temp_buf16, 16, b_key, b_iv, AES_ENCRYPT );  
        
        cipher_buffer = realloc( cipher_buffer, sizeof( bp_buffer_t) * (*cipher_buffer_size) + 16 );
        bp_buffer_t *seek_cb = (cipher_buffer + *cipher_buffer_size);
        *cipher_buffer_size += 16;
        memcpy( seek_cb, cipher, 16 );

        free( cipher );
        free( temp_buf16 );

        i += 16;
    }
       
    free( b_key );
    free( b_iv );
    free( hash_key );

    return cipher_buffer;
}

bp_buffer_t *old_decrypt_buffer( bp_buffer_t *buffer,    bp_size_t buffer_size,
                                 bp_buffer_t *key,       bp_size_t key_size,
                                 bp_size_t   *cipher_buffer_size ) {
    bp_buffer_t *hash_key   = hex_hash_key( key, key_size );
    bp_buffer_t *b_key      = get_aes_key_from_password( hash_key );
    bp_buffer_t *b_iv       = get_aes_iv_from_password( hash_key );

    bp_size_t last_block_size; // = temp_block_size == 0 ? 16 : temp_block_size;
    bp_buffer_t *cipher_buffer = calloc ( sizeof( bp_buffer_t ), 16 );
    *cipher_buffer_size = 0;

    int i = 0;
    while( i < buffer_size ) {
        bp_buffer_t *temp_buf16 = calloc( sizeof( bp_buffer_t ), 16 );
        bp_buffer_t *seek_buffer = (buffer + i);

        bp_size_t count = 16;
        if ( i + 16 >= buffer_size ) {
            memcpy( temp_buf16, seek_buffer, 16 );
            count = last_block_size;
        }
        else
            memcpy( temp_buf16, seek_buffer, 16 );

        if ( i == 0 ) { // service block read
            bp_buffer_t *cipher = openssl_crypt( temp_buf16, 16, b_key, b_iv, AES_DECRYPT );
            last_block_size = atoi( (char *)cipher );
            
            free( temp_buf16 );
            free( cipher );

            i += 16;

            continue;
        }

        bp_buffer_t *cipher = openssl_crypt( temp_buf16, 16, b_key, b_iv, AES_DECRYPT );  
        
        if ( i >= 16 )
            cipher_buffer = realloc( cipher_buffer, sizeof( bp_buffer_t) * (*cipher_buffer_size)+count );

        bp_buffer_t *seek_cb = cipher_buffer + *cipher_buffer_size;
        *cipher_buffer_size += count;
        memcpy( seek_cb, cipher, count );

        free( cipher );
        free( temp_buf16 );

        i += 16;
    }

    return cipher_buffer;
 }
