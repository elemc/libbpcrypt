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

bp_buffer_t *read_file( bp_buffer_t *filename, bp_size_t *filesize ) {
    int rd;
    rd = open ( (const char *)filename, O_RDONLY );
    if ( rd < 0 ) {
        perror( strerror( errno ) );
        return NULL;
    }
    ssize_t read_len, all_len = 0;
    ssize_t read_buf_size = 2048;
    bp_buffer_t *read_buffer = malloc( read_buf_size * sizeof( bp_buffer_t ) );
    bp_buffer_t *all_buffer = NULL;

    while ( (read_len = read( rd, read_buffer, read_buf_size )) > 0 ) {
        //all_len += read_len;
        all_buffer = realloc( all_buffer, (all_len + read_len) * sizeof(bp_buffer_t) );
        memcpy( all_buffer + all_len, read_buffer, read_len );
        all_len += read_len;
        if ( read_len != read_buf_size )
            break;
    }

    close(rd);

    free( read_buffer );

    *filesize = all_len;
    return all_buffer;
}

int main( int argc, char *argv[] ) {

    char *test_buffer = "Тестовый блок";
    bp_size_t test_buffer_size = strlen( test_buffer );

    char *test_key = "3510";
    bp_size_t test_key_size = strlen( test_key );

    bp_size_t test1_size;
    bp_buffer_t *test1 = old_encrypt_buffer( (bp_buffer_t *)test_buffer, test_buffer_size, (bp_buffer_t *)test_key, test_key_size, &test1_size );
    printf("[DEBUG] test=%s(%d)\n", test1, test1_size);

    bp_size_t hex_test1_size;
    bp_buffer_t *hex_test1 = BP_to_hex( test1, test1_size, &hex_test1_size );
    printf("Crypt buffer HEX: \"%s\"\n", hex_test1 ); 

    bp_size_t detest1_size;
    bp_buffer_t *detest1 = old_decrypt_buffer( (bp_buffer_t *)test1, test1_size, (bp_buffer_t *)test_key, test_key_size, &detest1_size );
    printf("[DEBUG] detest=%s(%d)\n", detest1, detest1_size);

    // test for decrypt file
    char *home_dir = getenv("HOME");
    char *basket_file = "/.basketpwd/default.cxml";
    bp_buffer_t *file_path = calloc( sizeof( bp_buffer_t ), strlen(home_dir) + strlen(basket_file) + 1 );
    sprintf( (char *)file_path, "%s%s", home_dir, basket_file );

    printf("[DEBUG] File for reading: %s\n", file_path );

    bp_size_t file_size;
    bp_buffer_t *file_content = read_file( file_path, &file_size );

    bp_size_t cfc_size;
    bp_buffer_t *clean_file_content = old_decrypt_buffer( file_content, file_size, (bp_buffer_t *)test_key, test_key_size, &cfc_size );

    int wd;
    wd = open( "default.xml", O_WRONLY | O_CREAT, 0644 );
    write( wd, clean_file_content, cfc_size );
    close( wd );

    return 0;
}
