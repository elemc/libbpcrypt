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

#include "misc.h"

#ifdef __cplusplus
extern "C"{
#endif 

typedef enum _BPRecordType {
    BP_RECORD_ROOT          = 0,
    BP_RECORD_FOLDER        = 1,
    BP_RECORD_PASSWORD      = 2,
    BP_RECORD_TEXT          = 3,
    BP_RECORD_FILE          = 4,
} BPRecordType;

typedef struct _BPRecord BPRecord;
struct _BPRecord {
    bp_buffer_t     *name;
    bp_buffer_t     *username;
    bp_buffer_t     *password;

    BPRecordType    type;

    bp_buffer_t     *data;
    bp_size_t       data_size;
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

extern void BPRecord_SetType( BPTreeRecord *record, BPRecordType type );
extern void BPRecord_SetName( BPTreeRecord *record, bp_buffer_t *name );

extern void BPRecord_SetFold( BPTreeRecord *record, bp_buffer_t *name );
extern void BPRecord_SetPassword( BPTreeRecord *record, bp_buffer_t *name, bp_buffer_t *login, bp_buffer_t *password );

// private
void *BPTreeRecord_Init ();
void BPTreeRecord_Final( BPTreeRecord *record );

#ifdef __cplusplus
}
#endif

#endif
