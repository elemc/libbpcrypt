// -*- C -*-
/* ------------------------------------ */
/* C source source                      */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */

/* Name: (bptree)
   Description: 
*/

#include "bptree.h"
#include <stdio.h>
#include <stdlib.h>

void _clear_pointer_( void *ptr );
void _clear_pointer_( void *ptr ) {
    if ( ptr == NULL )
        return;
    free( ptr );
    ptr = NULL;
}

void BPTree_Init( BPTree *tree ) {
    tree = calloc( sizeof(BPTree), 1 );
    if ( tree == NULL ) {
        perror("Tree object is NULL. Tree initialization failed.");
        return;
    }
    tree->root = BPTreeRecord_AddRecord( NULL );
    if ( tree->root == NULL )
        _clear_pointer_( tree );
}

void BPTree_Final( BPTree *tree ) {
    BPTreeRecord_Final( tree->root );
    tree->root = NULL;

    _clear_pointer_( tree );
}

void BPTreeRecord_Init( BPTreeRecord *record ) {
    record = calloc( sizeof(BPTreeRecord), 1 );
    if ( record == NULL ) {
        perror("Tree record object is NULL. Record initialization failed.");
        return;
    }

    record->child_count     = 0;
    record->first_child     = NULL;
    // record->last_child      = NULL;
    record->next_neighbor   = NULL;
    record->prev_neighbor   = NULL;
    record->parent          = NULL;
    record->record.username = NULL;
    record->record.password = NULL;
    record->record.comment  = NULL;
}

void BPTreeRecord_Final( BPTreeRecord *record ) {
    if ( record == NULL )
        return;

    // Change first child for parent
    BPTreeRecord *parent = record->parent;
    if ( parent != NULL ) {
        if ( record->parent->first_child == record ) {
            if ( record->next_neighbor != NULL )
                record->parent->first_child = record->next_neighbor;
            else
                record->parent->first_child = NULL;
        }
        parent->child_count -= 1;
    }

    // Final childs
    if ( record->child_count > 0 ) {
        BPTreeRecord_Final( record->first_child );
        record->first_child = NULL;
    }

    // Relink neighbor
    if ( record->next_neighbor != NULL && record->prev_neighbor != NULL ) {

    }

    // Clean BPRecord
    _clear_pointer_( record->record.username );
    _clear_pointer_( record->record.password );
    _clear_pointer_( record->record.comment );

    _clear_pointer_( record );
}

BPTreeRecord *BPTreeRecord_AddRecord( BPTreeRecord *parent ) {
    BPTreeRecord *rec;
    BPTreeRecord_Init( rec );
    if ( rec == NULL )
        return NULL;

    rec->parent = parent;
    if ( parent != NULL ) {
        if ( parent->last_child != NULL ) {
            parent->last_child->next_neighbor = rec;
            rec->prev_neighbor = parent->last_child;
            parent->last_child = rec;
        }

        parent->child_count += 1;
    }
    return rec;
}

