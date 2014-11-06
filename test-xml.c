// -*- C -*-
/* ------------------------------------ */
/* C source source                      */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */

/* Name: ()
   Description: 
*/

#include <libbpcrypt/xml.h>
#include <libbpcrypt/tree.h>

int main( int argc, char *argv[] )
{
    char *home_dir = getenv("HOME");
    char *basket_file = "/.basketpwd/default.cxml";
    bp_buffer_t *file_path = calloc( sizeof( bp_buffer_t ), strlen(home_dir) + strlen(basket_file) + 1 );
    sprintf( (char *)file_path, "%s%s", home_dir, basket_file );

    bp_size_t file_size;
    bp_buffer_t *file_content = BP_read_file( file_path, &file_size );
    
    free( file_path );

    int wd;
    wd = open( "test.cxml", O_CREAT | O_WRONLY, 0644 );
    if ( wd == -1 ) {
        free( file_content );
        error(600, 600, "File write failed. %s (test.cxml)\n", strerror(errno) );
        return 600;
    }
    else {
        write( wd, file_content, file_size );
        close( wd );
    }
    free( file_content );

    char *test_key = "3510";
    bp_size_t test_key_size = strlen( test_key );

    BPTree *tree = BP_xml_old_load_file( "test.cxml", test_key, test_key_size );

    if ( tree == NULL ) {        
        error( 602, 602, "Error!" );
        return 602;
    }

    BPTree_Final(tree);

    printf("BP_xml_old_load_file test pass. OK.\n");

    return 0;
}
