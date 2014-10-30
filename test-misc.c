// -*- C -*-
/* ------------------------------------ */
/* C source source                      */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */

/* Name: (test-misc)
   Description: 
*/

#include <libbpcrypt/misc.h>
#include <string.h>

int main ( int argc, char *argv[] ) {
    char *buf = "Тестовое сообщение";
    int len = strlen(buf);

    bp_buffer_t *test_buf = calloc( sizeof( bp_buffer_t ), len+1 );
    bp_size_t test_len = len;
    memcpy( test_buf, buf, len+1 ); 

    bp_buffer_t *hash = BP_md5_hash( test_buf, test_len );

    bp_size_t hex_len;
    bp_buffer_t *hex = BP_to_hex( hash, MD5_DIGEST_LENGTH, &hex_len );

    printf("For string \"%s\" hex is \"%s\"\n", test_buf, hex );

    free( hash );
    free( hex );
    free( test_buf );

    return 0;
}
