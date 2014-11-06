#include <libbpcrypt/crypt.h>
#include <libbpcrypt/misc.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

extern int errno;

char *decrypt_file_name( char *filename, bp_size_t *dfn_size ) {
    char add_file_name[] = ".decrypt";

    *dfn_size = strlen(filename) + strlen(add_file_name) + 1;
    char *temp_file_name = malloc( *dfn_size * sizeof( char ) );
    sprintf( temp_file_name, "%s%s", filename, add_file_name );

    return temp_file_name;
}

int stage2 ( char *file_for_read, char *file_for_write, bp_buffer_t *key, bp_size_t key_size ) {
    printf("Second stage. Files\n"); 
    bp_size_t buffer_len;
    bp_buffer_t *buffer = BP_read_file ( file_for_read, &buffer_len );
    if ( buffer == NULL )
        return 2;

    bp_buffer_t *new_all_buf;
    bp_size_t new_all_len;
    new_all_buf = encrypt_buffer ( buffer, buffer_len, key, key_size, &new_all_len );

    int wd;
    wd = open ( file_for_write, O_WRONLY | O_CREAT, 0644 );
    if ( wd < 0 ) {
        perror ( strerror ( errno ) );
        return errno;
    }
    write( wd, new_all_buf, new_all_len );

    close(wd);
    free( buffer );
    free( new_all_buf );

    return 0;
}

int stage3 ( char *file_for_read, bp_buffer_t *key, bp_size_t key_size ) {
    printf("Third stage. Decrypt files\n"); 

    bp_size_t ffw_size;
    char *file_for_write = decrypt_file_name( file_for_read, &ffw_size );
    printf("Decrypt file name is %s\n", file_for_write);

    bp_size_t buffer_len;
    bp_buffer_t *buffer = BP_read_file ( file_for_read, &buffer_len );
    if ( buffer == NULL ) {
        perror("Buffer is empty");
        return 3;
    }

    bp_size_t new_all_len;
    bp_buffer_t *new_all_buf = decrypt_buffer ( buffer, buffer_len, key, key_size, &new_all_len );

    int wd;
    wd = open ( file_for_write, O_WRONLY | O_CREAT, 0644 );
    if ( wd < 0 ) {
        perror ( strerror ( errno ) );
        return errno;
    }
    write( wd, new_all_buf, new_all_len );

    close(wd);
    free( buffer );
    free( new_all_buf );
    free( file_for_write );

    return 0;
}

unsigned char *half_stage4( char *filename ) {
    printf("Try to read '%s'\n", filename );

    bp_size_t buffer_len1;
    bp_buffer_t *buffer1 = BP_read_file ( filename, &buffer_len1 );

    if ( buffer1 == NULL ) {
        perror("Buffer is empty!");
        return NULL;
    }
    bp_buffer_t *md5sum = BP_md5_hash( buffer1, buffer_len1 );
    free( buffer1 );
    
    return md5sum;
}

int stage4 ( char *file1, char* file2 ) {
    printf("Four stage. Equal files\n"); 

    unsigned char *md5sum1, *md5sum2;

    md5sum1 = half_stage4( file1 );
    md5sum2 = half_stage4( file2 );

    if ( memcmp( md5sum1, md5sum2, MD5_DIGEST_LENGTH ) == 0 )
        printf("Checksums is identical. OK!\n");
    else
        printf("Checksums is not identical. Fail!\n");

    free( md5sum1 );
    free( md5sum2 );

    return 0;
}

int main ( int argc, char *argv[] ) {
    bp_buffer_t buf[] = "Я помню чудное мгновенье, передо мной явилась ты...";
    bp_size_t buf_size = strlen( (char *)buf );

    bp_buffer_t key[] = "Паролик";
    bp_size_t key_size = strlen( (char *)key );

    bp_buffer_t *cipher, *clean;
    bp_size_t cipher_len, clean_len;

    cipher = encrypt_buffer ( buf, buf_size, key, key_size, &cipher_len );
    clean = decrypt_buffer ( cipher, cipher_len, key, key_size, &clean_len );
    
    free( cipher );

    int cmpresult = strncmp( (char *)buf, (char *)clean, clean_len );
    if ( cmpresult == 0 )
        printf("It works! Done!\n");
    else
        printf( "It don't work! Fail! %d\n", cmpresult );

    if ( argc < 3 ) {
        perror("Stop here. Not given argument.");
        return 1;
    }
    // Second test - file
    bp_size_t dfn_size;
    char *dfn = decrypt_file_name( argv[2], &dfn_size );
    stage2( argv[1], argv[2], key, key_size );
    stage3( argv[2], key, key_size );
    stage4( argv[1], dfn );

    free( clean );
    free( dfn );

    return 0;
}
