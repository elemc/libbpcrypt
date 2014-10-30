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
    int return_result = 0;
    char *buf = "Тестовое сообщение";
    int len = strlen(buf);
    char *hex_hash_template="5e3d5c58fa308ad3fc40c94d4f9c79d2";

    bp_buffer_t *test_buf = calloc( sizeof( bp_buffer_t ), len+1 );
    bp_size_t test_len = len;
    memcpy( test_buf, buf, len+1 ); 

    bp_buffer_t *hash = BP_md5_hash( test_buf, test_len );

    bp_size_t hex_len;
    bp_buffer_t *hex = BP_to_hex( hash, MD5_DIGEST_LENGTH, &hex_len );

    if ( memcmp( hex, hex_hash_template, hex_len ) == 0 )
        printf("Hash and HEX works fine! OK.\n");
    else {
        printf("Hash or HEX works ugly! Fail.\n");
        return_result = 100;
    }

    printf("[DEBUG] For string \"%s\" hex is \"%s\"\n", test_buf, hex );

    free( hash );
    free( hex );
    free( test_buf );

    return return_result;
}
