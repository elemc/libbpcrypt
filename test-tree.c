// -*- C -*-
/* ------------------------------------ */
/* C source source                      */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */

/* Name: (test-tree)
   Description: 
*/

#include "bptree.h"
#include <stdio.h>
#include <stdlib.h>

int main( int argc, char *argv[] ) {

    BPTree *tree;
    BPTree_Init( tree );

    BPTreeRecord *root = tree->root;

    int i, j;
    for ( i=0; i < 1000; i++ ) {
        printf( "Head %d:\n", i );
        BPTreeRecord *head = BPTreeRecord_AddRecord( root );
        for( j=0; j < 100; j++ ) {
            printf( "\rChild %d:\n", j );
            BPTreeRecord *child = BPTreeRecord_AddRecord( head );
        }
    }

    BPTree_Final( tree );

    return 0;
}
