// -*- C -*-
/* ------------------------------------ */
/* C source source                      */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */

/* Name: (test-misc)
   Description: 
*/

#include <libbpcrypt/misc.h>
#include <fcntl.h>
#include <string.h>

int main ( int argc, char *argv[] ) {
    int return_result = 0;
    char *buf = "Тестовое сообщение";
    int len = strlen(buf);
    char *hex_hash_template="5e3d5c58fa308ad3fc40c94d4f9c79d2";
    char *hex_template="d0a2d0b5d181d182d0bed0b2d0bed0b520d181d0bed0bed0b1d189d0b5d0bdd0b8d0b5";

    bp_buffer_t *test_buf = calloc( sizeof( bp_buffer_t ), len+1 );
    bp_size_t test_len = len;
    memcpy( test_buf, buf, len+1 ); 

    bp_buffer_t *hash = BP_md5_hash( test_buf, test_len );

    bp_size_t hex_len;
    bp_buffer_t *hex = BP_to_hex( hash, MD5_DIGEST_LENGTH, &hex_len );

    if ( memcmp( hex, hex_hash_template, hex_len ) == 0 )
        printf("BP_md5_hash test pass. OK.\n");
    else {
        printf("BP_md5_hash test fail. Fail!\n");
        return_result = 100;
    }

    bp_size_t hex_b_len;
    bp_buffer_t *hex_buffer = BP_to_hex( test_buf, test_len, &hex_b_len );

    if ( memcmp( hex_template, hex_buffer, hex_b_len ) == 0 )
        printf("BP_to_hex test pass. OK.\n");
    else {
        printf("BP_to_hex test fail. Fail!\n");
        return_result = 200;
    }

    char *test_file = "default-t.cxml";

    int wd;
    wd = open( test_file, O_WRONLY | O_CREAT, 0644 );
    if ( wd == -1 ) {
        printf( "Error open %s file for write (%d)!", test_file, errno );
        return_result = 300;
    }
    else {
        char *test_msg_text = "This is test buffer";
        write(wd, test_msg_text, strlen(test_msg_text) );
        close( wd );

        int test_msg_size;
        bp_buffer_t *test_msg = BP_read_file( test_file, strlen(test_file), &test_msg_size );

        int result = 0;
        result += memcmp(test_msg_text, test_msg, test_msg_size);        
        result += strlen(test_msg_text) - test_msg_size;

        if ( result == 0 )
            printf("BP_read_file test pass. OK.\n");
        else
            printf("BP_read_file test fail. Fail!\n");

        return_result = result;

        free( test_msg );
    }

    free( hash );
    free( hex );
    free( test_buf );

    return return_result;
}
