// -*- C -*-
/* ------------------------------------ */
/* C source source                      */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */

/* Name: (misc)
   Description: 
*/

#include "misc.h"
#include <stdarg.h>
#include "config.h"

bp_buffer_t *BP_md5_hash( bp_buffer_t *buffer, bp_size_t buffer_size ) {

    bp_buffer_t *result_hash = calloc( sizeof( bp_buffer_t) , MD5_DIGEST_LENGTH );
    if ( result_hash == NULL ) {
        perror( "Initialization result_hash failed." );
        return NULL;
    }

    MD5_CTX md5_context;
    MD5_Init( &md5_context );
    MD5_Update( &md5_context, buffer, buffer_size );
    MD5_Final( result_hash, &md5_context );

    return result_hash;
}

bp_buffer_t *BP_to_hex( bp_buffer_t *buffer, bp_size_t buffer_size, bp_size_t *hex_size ) {
    *hex_size = buffer_size * 2 + 1;
    bp_buffer_t *result_hex = calloc( sizeof( bp_buffer_t ), *hex_size );
    
    int i;
    for ( i = 0; i < buffer_size; i++ ) 
        sprintf( (char *)(result_hex+i*2), "%02x", buffer[i]);
    result_hex[*hex_size - 1] = '\0';

    return result_hex;
}

bp_buffer_t *BP_read_file( const char *filename, bp_size_t *file_size )
{
    int rd;
    rd = open( filename, O_RDONLY );
    if ( rd  < 0 ) {

        BP_error( 400, errno, "Read file \"%s\" failed (%d).", filename, errno );
        return NULL;
    }

    ssize_t read_len, all_len = 0;
    ssize_t read_buf_size = BP_READ_BLOCK_SIZE;
   
    bp_buffer_t *read_buffer = calloc( sizeof( bp_buffer_t), read_buf_size );
    bp_buffer_t *all_buffer = NULL;

    while ( (read_len = read( rd, read_buffer, read_buf_size )) > 0 ) {
        all_buffer = realloc( all_buffer, (all_len + read_len) * sizeof(bp_buffer_t) );
        memcpy( all_buffer + all_len, read_buffer, read_len );
        all_len += read_len;
        if ( read_len != read_buf_size )
            break;
    }

    close(rd);
    free( read_buffer );

    *file_size = all_len;
    return all_buffer;
}

bp_buffer_t *BP_ncopy_ptr( bp_buffer_t *ptr, bp_size_t size )
{
    bp_buffer_t *new_ptr = calloc( sizeof( bp_buffer_t ), size );
    memcpy( new_ptr, ptr, size );
    return new_ptr;
}

bp_buffer_t *BP_copy_ptr( bp_buffer_t *ptr )
{
    bp_size_t ptr_size = strlen( (const char *)ptr ) + 1;
    return BP_ncopy_ptr( ptr, ptr_size );
}

void BP_error( int code, int eval, const char *fmt, ... )
{
    va_list ap;
    va_start( ap, fmt );    

    char *msg = calloc( sizeof( char ), 1024 );
    if ( msg != NULL ) {
        // int size = 
        vsprintf( msg, fmt, ap );
#if defined(_WIN32) || defined(_WIN64)
        printf( "%s\n", msg );
#else
        err( eval, "%s", msg );
#endif
        free( msg );
    }
    va_end( ap );
}

bp_buffer_t *BP_from_hex( bp_buffer_t *buffer, bp_size_t buffer_size, bp_size_t *data_size )
{
    *data_size = 0;

    bp_buffer_t *result = calloc( sizeof( bp_buffer_t ), buffer_size / 2 );
    bp_buffer_t *dst = result;

    int i = 0;

    while ( i < buffer_size ) {
        uint32_t value;
        if ( sscanf( buffer, "%02x", &value ) == 1 ) {
            buffer += 2;
            *dst++ = value;
            *data_size += 1;
        }
        i += 2;
    }

    return result;
}
