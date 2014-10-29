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
#include <libbpcrypt/crypt.h>

int main( int argc, char *argv[] ) {

    BPTree *tree;
    BPTree_Init( tree );

    printf("PTR: %p//%p\n", tree, tree->root);
    //BPTreeRecord *root = BPTreeRecord_AddRecord( NULL );// tree->root;
    //tree->root = root;
    BPTreeRecord *root = tree->root;

    int i, j;
    for ( i=0; i < 1000; i++ ) {
        //printf( "Head %d:\t%p\t%p\n", i, root, tree->root );
        printf("\n");
        BPTreeRecord *head = BPTreeRecord_AddRecord( root );
        for( j=0; j < 100; j++ ) {
            printf( "\rHead %d: Child %d", i, j );
            BPTreeRecord *child = BPTreeRecord_AddRecord( head );
        }
    }

    BPTree_Final( tree );

    return 0;
}
