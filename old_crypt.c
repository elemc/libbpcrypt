// -*- C -*-
/* ------------------------------------ */
/* C source source                      */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */

/* Name: (old_crypt)
   Description: 
*/

#include "old_crypt.h"

bp_buffer_t *old_encrypt_buffer( bp_buffer_t *buffer,    bp_size_t buffer_size,
                                 bp_buffer_t *key,       bp_size_t key_size,
                                 bp_size_t   *cipher_buffer_size ) {
    
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
