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

bp_buffer_t *hex_hash_key( bp_buffer_t *key, bp_size_t key_size ) {
    bp_buffer_t *hash_key = BP_md5_hash( key, key_size );
    bp_size_t hh_key_size;
    bp_buffer_t *hex_hash_key = BP_to_hex( hash_key, MD5_DIGEST_LENGTH, &hh_key_size );

    free( hash_key );
    return hex_hash_key;
}

bp_buffer_t *get_aes_key_from_password( bp_buffer_t *hex_hash_key ) {
    bp_buffer_t *b_key = calloc( sizeof( bp_buffer_t ), 16 );
    memcpy( (void *)b_key, hex_hash_key, 16 );

    return b_key;
}

bp_buffer_t *get_aes_iv_from_password( bp_buffer_t *hex_hash_key ) {
    bp_buffer_t *b_iv = calloc( sizeof( bp_buffer_t ), 16 );
    memcpy( (void *)b_iv, hex_hash_key+16, 16 );

    return b_iv;
}

bp_buffer_t *old_encrypt_buffer( bp_buffer_t *buffer,    bp_size_t buffer_size,
                                 bp_buffer_t *key,       bp_size_t key_size,
                                 bp_size_t   *cipher_buffer_size ) {
    bp_buffer_t *hash_key   = hex_hash_key( key, key_size );
    bp_buffer_t *b_key      = get_aes_key_from_password( hash_key );
    bp_buffer_t *b_iv       = get_aes_iv_from_password( hash_key );


   
    free( b_key );
    free( b_iv );
    free( hash_key );
    
 /*   
    // Превращаем пароль-хэш в ключ/вектор
    // Разбиваем хэш на 2 части
    if ( pwd.length() != 32 ) {// т.е. если пароль не является HEX кодом хеш пароля
        return NULL;
    }

    QByteArray key  = QByteArray().append(pwd.left(16));//QByteArray::fromHex(pwd.toUtf8());//
    QByteArray iv   = QByteArray().append(pwd.right(16));//QByteArray::fromHex(pwd.toUtf8());//

    QByteArray cipherBuffer;

    //==========================================================================================
    // Служебный блок данных
    // Вычисляем размер последнего блока данных
    int tempBlockSize = buf.size() % 16;
    int lastBlockSize = tempBlockSize == 0 ? 16 : tempBlockSize;
    QByteArray serviceBlock = QByteArray( QVariant(lastBlockSize).toByteArray() );
    while ( serviceBlock.size() < 16 )
        serviceBlock.append('\0');
    char *cipher = crypt16( serviceBlock.data(), serviceBlock.size(), key.data(), iv.data() );

    if (cipher != NULL) {
        QByteArray ciph16 = QByteArray( cipher, 16 );
        cipherBuffer.append(ciph16);
        //free (cipher);
        delete [] cipher;
    }
    else {
        return NULL;
    }
    // Конец служебного блока данных
    //==========================================================================================



    // Создаем цикл перебора буфера данных по 16 байт
    for ( int i = 0; i < buf.size(); i+=16 )
    {
        QByteArray buf16;// = QByteArray::
        if (i + 16 <= buf.size())
            buf16 = buf.mid(i, 16);
        else {
            buf16 = buf.mid(i);
        }

        if ( buf16.size() == 16 )
            ;//buf16.append('\0');
        else {
            while ( buf16.size() < 15 )
                buf16.append('\0');
            buf16.append( buf.size() - i );
        }

        char *cipher = crypt16( buf16.data(), buf16.size(), key.data(), iv.data() );

        if (cipher != NULL) {
            QByteArray ciph16 = QByteArray( cipher, 16 );
            cipherBuffer.append(ciph16);
            delete [] cipher;
        }
    }

    return cipherBuffer;
*/    
}
