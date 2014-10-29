// -*- C -*-
/* ------------------------------------ */
/* C source header                      */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */

/* Name: (bptree)
   Description: 
*/
#ifndef TREE_H
#define TREE_H

#include "crypt.h"

#ifdef __cplusplus
extern "C"{
#endif 

typedef struct _BPRecord BPRecord;
struct _BPRecord {
    bp_buffer_t     *username;
    bp_buffer_t     *password;
    bp_buffer_t     *comment;
};

typedef struct _BPTreeRecord BPTreeRecord;
struct _BPTreeRecord {
    bp_size_t          child_count;
    BPTreeRecord      *first_child;
    BPTreeRecord      *last_child;
    BPTreeRecord      *prev_neighbor;
    BPTreeRecord      *next_neighbor;
    BPTreeRecord      *parent;
    BPRecord           record;
};

typedef struct _BPTree      BPTree;
struct _BPTree {
    BPTreeRecord     *root;
};

// public
extern void *BPTree_Init();
extern void BPTree_Final( BPTree *tree );

extern void *BPTreeRecord_AddRecord( BPTreeRecord *parent );
extern void *BPTreeRecord_InsertRecordAfter( BPTreeRecord *after_record );
extern void *BPTreeRecord_InsertRecordBefore( BPTreeRecord *before_record );

// private
void *BPTreeRecord_Init ();
void BPTreeRecord_Final( BPTreeRecord *record );

#ifdef __cplusplus
}
#endif

#endif
