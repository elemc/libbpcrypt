// -*- C -*-
/* ------------------------------------ */
/* C source source                      */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */

/* Name: (test-tree)
   Description: 
*/

#include <libbpcrypt/tree.h>
#include <stdio.h>
#include <stdlib.h>

int main( int argc, char *argv[] ) {

    printf("Begin...\n");
    BPTree *tree = BPTree_Init();
    printf("[DEBUG] after init: %p\n", tree);

    printf("[DEBUG] Step 1\n");

    printf("PTR: %p/\n", tree);
    printf("PTR: %p//%p\n", tree, tree->root);
    BPTreeRecord *root = tree->root;

    int i, j;
    for ( i=0; i < 1000; i++ ) {
       BPTreeRecord *head = BPTreeRecord_AddRecord( root );
        for( j=0; j < 100; j++ ) {
            printf( "\rHead %d: Child %d", i, j );
            BPTreeRecord *child = BPTreeRecord_AddRecord( head );
        }
    }

    //BPTree_Final( tree );

    return 0;
}
