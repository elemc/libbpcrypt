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

void add_childs( BPTreeRecord *parent, unsigned int count ) {
    int j;
    for( j = 0; j < count; j++ ) {
        printf( "\rChild %d of %d", j, count );
        BPTreeRecord *child = BPTreeRecord_AddRecord( parent );

        if ( j%7 == 0 && j!=0 ) {
            printf("\n[DEBUG] Ok. Before %d %% 7 == 0\n", j);
            BPTreeRecord *left_child = BPTreeRecord_InsertRecordBefore( child );
            add_childs( left_child, 6 );
        }
    }
    printf("\n");
}

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

        if ( i%7 == 0 && i != 0 ) {
            printf("[DEBUG] Ok. After %d %% 7 == 0\n", i);
            BPTreeRecord *right_head = BPTreeRecord_InsertRecordAfter( head );
            add_childs( right_head, 6 );
        }
        add_childs( head, 1000 );
    }

    BPTree_Final( tree );
    printf("\n");

    return 0;
}
