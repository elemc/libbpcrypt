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
#include <sys/mman.h>
#include <unistd.h>

#include <malloc/malloc.h>

void *(*system_malloc)(malloc_zone_t *zone, size_t size);
void (*system_free)(malloc_zone_t *zone, void *ptr);
void *(*system_calloc)(malloc_zone_t *zone, size_t num_items, size_t size);
void (*system_free_ds)(malloc_zone_t *zone, void *ptr, size_t size);

uint64_t memory_count;
static malloc_zone_t original_zone;

void add_childs( BPTreeRecord *parent, unsigned int count ) {
    int j;
    for( j = 0; j < count; j++ ) {
        printf( "\r\t\tChild %d of %d", j, count );
        BPTreeRecord *child = BPTreeRecord_AddRecord( parent );

        if ( j%7 == 0 && j!=0 ) {
            BPTreeRecord *left_child = BPTreeRecord_InsertRecordBefore( child );
            add_childs( left_child, 6 );
        }
    }
}

void *my_malloc( malloc_zone_t *zone,size_t size ) {
    memory_count += 1;
    void *ptr = system_malloc( zone, size );
    return ptr;
}

void *my_calloc( malloc_zone_t *zone, size_t num_items, size_t size) {
    memory_count += 1;
    void *ptr = system_calloc( zone, num_items, size );
    return ptr;
}

void my_free( malloc_zone_t *zone, void *ptr ) {
    memory_count -= 1;
    (*original_zone.free)(zone, ptr);
}
void my_free_ds( malloc_zone_t *zone, void *ptr, size_t size ) {
    memory_count -= 1;
    (*original_zone.free_definite_size)(zone, ptr, size);
}


void my_malloc_init() {
    malloc_zone_t *zone = malloc_default_zone();
    original_zone = *zone;

    system_malloc   = zone->malloc;
    system_free     = zone->free;
    system_calloc   = zone->calloc;
    system_free_ds  = zone->free_definite_size;
    if (mprotect(zone, getpagesize(), PROT_READ | PROT_WRITE) != 0) {
        perror("munprotect failed");
        abort();
    }

    zone->malloc    = &my_malloc;
    zone->free      = &my_free;
    zone->calloc    = &my_calloc;
    zone->free_definite_size    = &my_free_ds;
 
    if (mprotect(zone, getpagesize(), PROT_READ) != 0) {
        perror("mprotect failed");
        abort();
    }
 }

int main( int argc, char *argv[] ) {

    memory_count = 0;
    my_malloc_init();

    BPTree *tree = BPTree_Init();
    BPTreeRecord *root = tree->root;

    int i, j;
    for ( i=0; i < 100; i++ ) {
        BPTreeRecord *head = BPTreeRecord_AddRecord( root );
        printf( "\rHead %d", i );

        if ( i%7 == 0 && i != 0 ) {
            BPTreeRecord *right_head = BPTreeRecord_InsertRecordAfter( head );
            add_childs( right_head, 6 );
        }
        add_childs( head, 100 );
    }

    printf("\nMemory allocated: %llu\n", memory_count);
    BPTree_Final( tree );
    //printf("Memory count after final: %llu\n", memory_count);
    if ( memory_count == 1 ) 
        printf("Memory is clean. OK!\n");

    return 0;
}
