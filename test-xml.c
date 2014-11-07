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
    char *basket_file = "test.cxml";

    bp_size_t file_size;
    bp_buffer_t *file_content = BP_read_file( basket_file, &file_size );
    
    char *test_write_filename = "test_write.cxml";
    int wd;
    wd = open( test_write_filename, O_CREAT | O_WRONLY, 0644 );
    if ( wd == -1 ) {
        free( file_content );
        BP_error(600, 600, "File write failed. %s (%s)\n", strerror(errno), test_write_filename );
        return 600;
    }
    else {
        write( wd, file_content, file_size );
        close( wd );
    }
    free( file_content );

    char *test_key = "1234";
    bp_size_t test_key_size = strlen( test_key );

#ifdef WITH_OLD_CRYPT
    // Stage 1. Loading old cxml to tree
    BPTree *tree = BP_xml_old_load_file( "test.cxml", (bp_buffer_t *)test_key, test_key_size );
    if ( tree == NULL ) {        
        BP_error( 602, 602, "Error!" );
        return 602;
    }

    BPTree_Final(tree);
    printf("BP_xml_old_load_file test pass. OK.\n");
#else
    printf("BP_xml_old_load_file not found. OK.\n");
#endif

    return 0;
}
