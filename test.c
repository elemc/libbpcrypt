#include <libbpcrypt.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

extern int errno;

bp_buffer_t *read_file( char *filename, bp_size_t *filesize ) {
    int rd;
    rd = open ( filename, O_RDONLY );
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

int stage2 ( char *file_for_read, char *file_for_write, bp_buffer_t *key, bp_size_t key_size ) {
    printf("Second stage. Files\n"); 
    bp_size_t buffer_len;
    bp_buffer_t *buffer = read_file ( file_for_read, &buffer_len );
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

    char add_file_name[] = ".decrypt";
    char *file_for_write = malloc( (strlen(file_for_read) + strlen(add_file_name) + 1) * sizeof( char ) );
    sprintf( file_for_write, "%s%s", file_for_read, add_file_name );
    printf("Decrypt file name is %s\n", file_for_write);

    bp_size_t buffer_len;
    bp_buffer_t *buffer = read_file ( file_for_read, &buffer_len );
    if ( buffer == NULL ) {
        perror("Buffer is empty");
        return 3;
    }

    bp_buffer_t *new_all_buf;
    bp_size_t new_all_len;
    new_all_buf = decrypt_buffer ( buffer, buffer_len, key, key_size, &new_all_len );

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

int main ( int argc, char *argv[] ) {
    bp_buffer_t buf[] = "Я помню чудное мгновенье, передо мной явилась ты...";
    bp_size_t buf_size = strlen( (char *)buf );

    bp_buffer_t key[] = "Паролик";
    bp_size_t key_size = strlen( (char *)key );

    bp_buffer_t *cipher, *clean;
    bp_size_t cipher_len, clean_len;

    cipher = encrypt_buffer ( buf, buf_size, key, key_size, &cipher_len );
    clean = decrypt_buffer ( cipher, cipher_len, key, key_size, &clean_len );
    
    printf("Original text \"%s\" (%d)\n", buf, buf_size);
    printf("Encrypted text \"%s\" (%d)\n", cipher, cipher_len );
    printf("Decrypted text \"%s\" (%d)\n", clean, clean_len );

    free( cipher );
    free( clean );

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
    stage2( argv[1], argv[2], key, key_size );
    stage3( argv[2], key, key_size );
}
