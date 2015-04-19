#include <openssl/evp.h>
#include <openssl/aes.h>
#include "crypt.h"
#include "config.h"

uint8_t bp_aes_init_common ( EVP_CIPHER_CTX *context, bp_buffer_t *key, bp_size_t key_size, bp_buffer_t *aes_key, bp_buffer_t *aes_vector );
uint8_t bp_aes_init_encrypt ( EVP_CIPHER_CTX *context, bp_buffer_t *key, bp_size_t key_size );
uint8_t bp_aes_init_decrypt ( EVP_CIPHER_CTX *context, bp_buffer_t *key, bp_size_t key_size );

bp_buffer_t *encrypt_buffer( bp_buffer_t *buffer, bp_size_t buffer_size, 
                             bp_buffer_t *key, bp_size_t key_size,
                             bp_size_t *cipher_buffer_size) {
    int result;

    // init aes encrypt context
    EVP_CIPHER_CTX en_context;
    result = bp_aes_init_encrypt ( &en_context, key, key_size );
    if ( result != 0 ) {
        return NULL;
    }

    /* max ciphertext len for a n bytes of plaintext is n + AES_BLOCK_SIZE -1 bytes */
    bp_size_t cipher_size = buffer_size + AES_BLOCK_SIZE; // - 1;
    bp_size_t final_size = 0;
    
    bp_buffer_t *cipher_buffer = calloc( sizeof( bp_buffer_t), cipher_size );

    /* allows reusing of 'e' for multiple encryption cycles */ //FIXME: WTF?
    EVP_EncryptInit_ex ( &en_context, NULL, NULL, NULL, NULL );

    /* update ciphertext, c_len is filled with the length of ciphertext generated,
        *len is the size of plaintext in bytes */
    EVP_EncryptUpdate ( &en_context, cipher_buffer, &cipher_size, buffer, buffer_size );

    /* update ciphertext with the final remaining bytes */
    EVP_EncryptFinal_ex ( &en_context, cipher_buffer + cipher_size, &final_size);

    *cipher_buffer_size = cipher_size + final_size;

    EVP_CIPHER_CTX_cleanup( &en_context );

    return cipher_buffer;
}

bp_buffer_t *decrypt_buffer( bp_buffer_t *buffer, bp_size_t buffer_size,
                             bp_buffer_t *key, bp_size_t key_size,
                             bp_size_t *clean_buffer_size ) {
    int result;
    
    // init aes encrypt context
    EVP_CIPHER_CTX de_context;
    result = bp_aes_init_decrypt ( &de_context, key, key_size );
    if ( result != 0 ) {
        return NULL;
    }

    bp_size_t clean_size = buffer_size, final_size = 0;
    bp_size_t cb_size = clean_size;
    bp_buffer_t *clean_buffer = calloc( sizeof( bp_buffer_t), cb_size );
  
    EVP_DecryptInit_ex( &de_context, NULL, NULL, NULL, NULL);
    EVP_DecryptUpdate( &de_context, clean_buffer, &clean_size, buffer, buffer_size );
    EVP_DecryptFinal_ex( &de_context, clean_buffer + clean_size, &final_size );

    *clean_buffer_size = clean_size + final_size;

    EVP_CIPHER_CTX_cleanup( &de_context );

    return clean_buffer;
}

uint8_t bp_aes_init_common ( EVP_CIPHER_CTX *context, bp_buffer_t *key, bp_size_t key_size, bp_buffer_t *aes_key, bp_buffer_t *aes_vector ) {
    
    int result;
    int iteration_count = key_size; // FIXME: It's workaround

    result = EVP_BytesToKey( EVP_aes_256_cbc(), EVP_sha256(), NULL, key, key_size, iteration_count, aes_key, aes_vector );
    if ( result != 32 ) {
        perror( "Key size should be 256 bits" );
        return -1;
    }

    EVP_CIPHER_CTX_init ( context );
    return 0;
}

uint8_t bp_aes_init_encrypt ( EVP_CIPHER_CTX *context, bp_buffer_t *key, bp_size_t key_size ) {

    int result;

    bp_buffer_t aes_key[32], aes_vector[32];

    result = bp_aes_init_common( context, key, key_size, aes_key, aes_vector );

    if ( result != 0 ) {
        return result;
    }

    EVP_EncryptInit_ex ( context, EVP_aes_256_cbc(), NULL, aes_key, aes_vector );

    return 0;
}

uint8_t bp_aes_init_decrypt ( EVP_CIPHER_CTX *context, bp_buffer_t *key, bp_size_t key_size ) {

    int result;

    bp_buffer_t aes_key[32], aes_vector[32];

    result = bp_aes_init_common( context, key, key_size, aes_key, aes_vector );

    if ( result != 0 ) {
        return result;
    }

    EVP_DecryptInit_ex ( context, EVP_aes_256_cbc(), NULL, aes_key, aes_vector );

    return 0;
}


