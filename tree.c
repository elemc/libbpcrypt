// -*- C -*-
/* ------------------------------------ */
/* C source source                      */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */

/* Name: (bptree)
   Description: 
*/

#include "tree.h"
#include <stdio.h>
#include <stdlib.h>

void _clear_pointer_( void *ptr ) {
    if ( ptr == NULL )
        return;
    free( ptr );
    ptr = NULL;
}

void *BPTree_Init() {
    BPTree *t = calloc( sizeof(BPTree), 1 );
    if ( t == NULL ) {
        perror("Tree object is NULL. Tree initialization failed.");
        return NULL;
    }
    BPTreeRecord *root = BPTreeRecord_AddRecord( NULL );
    if ( root == NULL ) {
        _clear_pointer_( t );
        perror("Error in root record initialization.");
        return NULL;
    }
    t->root = root;
    return t;
}

void BPTree_Final( BPTree *tree ) {
    BPTreeRecord_Final( tree->root );
    tree->root = NULL;

    _clear_pointer_( tree );
}

void *BPTreeRecord_Init() {
    BPTreeRecord *record;
    record = calloc( sizeof(BPTreeRecord), 1 );
    if ( record == NULL ) {
        perror("Tree record object is NULL. Record initialization failed.");
        return NULL;
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

    return record;
}

void BPTreeRecord_Final( BPTreeRecord *record ) {
    if ( record == NULL )
        return;

    //printf( "\nChild count: %d\n", record->child_count );

    // Final childs
    if ( record->child_count > 0 ) {
        while( record->first_child != NULL ) 
            BPTreeRecord_Final( record->first_child );
    }

    // Change first child for parent
    if ( record->parent != NULL ) {
        BPTreeRecord *parent = record->parent;
        if ( record->parent->first_child == record ) {
            if ( record->next_neighbor != NULL )
                record->parent->first_child = record->next_neighbor;
            else
                record->parent->first_child = NULL;
        }
        parent->child_count -= 1;
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

void *BPTreeRecord_AddRecord( BPTreeRecord *parent ) {
    BPTreeRecord *rec = BPTreeRecord_Init();
    if ( rec == NULL ) 
        return NULL;

    if ( parent != NULL ) {
        rec->parent = parent;
        if ( parent->last_child != NULL ) {
            parent->last_child->next_neighbor = rec;
            rec->prev_neighbor = parent->last_child;
            parent->last_child = rec;
        }
        if ( parent->first_child == NULL ) {
            parent->first_child = rec;
            parent->last_child = rec;
        }

        parent->child_count += 1;
    }
    return rec;
}

void *BPTreeRecord_InsertRecordAfter( BPTreeRecord *after_record ) {
    if ( after_record == NULL ) {
        perror("Insert after record is NULL");
        return NULL;
    }
    BPTreeRecord *parent = after_record->parent;

    BPTreeRecord *rec = BPTreeRecord_Init();
    if ( rec == NULL )
        return NULL;

    if ( after_record->next_neighbor != NULL ) {
        BPTreeRecord *next = after_record->next_neighbor;

        rec->next_neighbor = next;
        next->prev_neighbor = rec;
    }

    rec->prev_neighbor = after_record;
    after_record->next_neighbor = rec;

    if ( parent != NULL ) {
        rec->parent = parent;
        if ( parent->last_child == after_record ) {
            parent->last_child = rec;
        }
        parent->child_count += 1;
    }

    return rec;
}

void *BPTreeRecord_InsertRecordBefore( BPTreeRecord *before_record ) {
    if ( before_record == NULL ) {
        perror("Insert before record is NULL");
        return NULL;
    }
    BPTreeRecord *parent = before_record->parent;

    BPTreeRecord *rec = BPTreeRecord_Init();
    if ( rec == NULL )
        return NULL;

    if ( before_record->prev_neighbor != NULL ) {
        BPTreeRecord *prev = before_record->prev_neighbor;

        rec->prev_neighbor = prev;
        prev->next_neighbor = rec;
    }

    rec->next_neighbor = before_record;
    before_record->prev_neighbor = rec;

    if ( parent != NULL ) {
        rec->parent = parent;
        if ( parent->first_child == before_record ) {
            parent->first_child = rec;
        }
        parent->child_count += 1;
    }

    return rec;
}


