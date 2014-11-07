// -*- C -*-
/* ------------------------------------ */
/* C source source                      */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */

/* Name: (test-old-crypt)
   Description: 
*/

#include <libbpcrypt/old_crypt.h>
#include <libbpcrypt/misc.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

int main( int argc, char *argv[] ) {

    int return_result = 0;

    char *test_buffer = "Тестовый блок";
    bp_size_t test_buffer_size = strlen( test_buffer );

    char *test_key = "1234";
    bp_size_t test_key_size = strlen( test_key );

    char *hex_crypt_test = "2f12e35b074804f6b989c826fed2a5dc3e63c0f61b94e13e9d72009f7bc4e1c7be69b6ec0089af620fccb4e36d330b48";

    // Stage 1. Crypt.
    bp_size_t test1_size;
    bp_buffer_t *test1 = old_encrypt_buffer( (bp_buffer_t *)test_buffer, test_buffer_size, (bp_buffer_t *)test_key, test_key_size, &test1_size );

    bp_size_t hex_test1_size;
    bp_buffer_t *hex_test1 = BP_to_hex( test1, test1_size, &hex_test1_size );
    printf( "[DEBUG] hex_test1=\"%s\"\n", hex_test1 );

    if ( memcmp( hex_test1, hex_crypt_test, hex_test1_size ) == 0 )
        printf("Stage 1. Crypt test success. OK\n");
    else {
        printf("Stage 1. Crypt test failed. Fail\n");
        return_result = 100;
    }

    // Stage 2. Decrypt
    bp_size_t detest1_size;
    bp_buffer_t *detest1 = old_decrypt_buffer( (bp_buffer_t *)test1, test1_size, (bp_buffer_t *)test_key, test_key_size, &detest1_size );

    if ( memcmp( detest1, test_buffer, detest1_size ) == 0 )
        printf("Stage 2. Decrypt test success. OK\n");
    else {
        printf("Stage 2. Decrypt test failed. Fail\n");
        return_result = 200;
    }
 
    free( test1 );
    free( hex_test1 );
    free( detest1 );

    // Stage 3. File decrypt
    // test for decrypt file
    char *basket_file = "test.cxml";

    bp_size_t file_size;    
    bp_buffer_t *file_content = BP_read_file( (const char *)basket_file, &file_size );
    bp_buffer_t *clean_file_content = NULL;
    bp_size_t cfc_size;
    
    if ( file_content == NULL ) {
        printf( "Stage 3. File decrypt failed. %s (%s)\n", strerror(errno), basket_file );
        return_result = 300;
    }
    else {
        clean_file_content = old_decrypt_buffer( file_content, file_size, (bp_buffer_t *)test_key, test_key_size, &cfc_size );
    }

    char *test_xml = "test.xml";
    int wd;
    wd = open( "test.xml", O_WRONLY | O_CREAT, 0644 );
    if ( wd == -1 ) {
        printf( "Stage 3. File decrypt failed. %s (%s)\n", strerror(errno), test_xml );
        return_result = 400;
    }
    else if ( clean_file_content != NULL ){
        write( wd, clean_file_content, cfc_size );
        close( wd );
        printf("Stage 3. File decrypt waiting. Please check \"%s\" file...\n", test_xml);
    }
    
    free( file_content );
    free( clean_file_content );

    return return_result;
}
