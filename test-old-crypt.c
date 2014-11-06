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

    char *test_key = "3510";
    bp_size_t test_key_size = strlen( test_key );

    char *hex_crypt_test = "e53b332042f2e5203f562f1e09e89113975dc9a898c094b1bdd992353a16061a674e061f7269195e57355527dab69f4b";

    // Stage 1. Crypt.
    bp_size_t test1_size;
    bp_buffer_t *test1 = old_encrypt_buffer( (bp_buffer_t *)test_buffer, test_buffer_size, (bp_buffer_t *)test_key, test_key_size, &test1_size );

    bp_size_t hex_test1_size;
    bp_buffer_t *hex_test1 = BP_to_hex( test1, test1_size, &hex_test1_size );

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
    char *home_dir = getenv("HOME");
    char *basket_file = "/.basketpwd/default.cxml";
    bp_buffer_t *file_path = calloc( sizeof( bp_buffer_t ), strlen(home_dir) + strlen(basket_file) + 1 );
    sprintf( (char *)file_path, "%s%s", home_dir, basket_file );

    bp_size_t file_size;
    bp_buffer_t *file_content = BP_read_file( file_path, &file_size );
    bp_buffer_t *clean_file_content = NULL;
    bp_size_t cfc_size;
    
    if ( file_content == NULL ) {
        printf( "Stage 3. File decrypt failed. %s (%s)\n", strerror(errno), file_path );
        return_result = 300;
    }
    else {
        clean_file_content = old_decrypt_buffer( file_content, file_size, (bp_buffer_t *)test_key, test_key_size, &cfc_size );
    }

    int wd;
    wd = open( "default.xml", O_WRONLY | O_CREAT, 0644 );
    if ( wd == -1 ) {
        printf( "Stage 3. File decrypt failed. %s (default.xml)\n", strerror(errno) );
        return_result = 400;
    }
    else if ( clean_file_content != NULL ){
        write( wd, clean_file_content, cfc_size );
        close( wd );
        printf("Stage 3. File decrypt waiting. Please check \"default.xml\" file...\n");
    }
    
    free( file_path );
    free( file_content );
    free( clean_file_content );

    return return_result;
}
