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
//#include <sys/mman.h>
#include <unistd.h>

uint64_t memory_count;

#ifdef __APPLE__
#include <sys/mman.h>
#define MEMORY_CLEAN 1

#include <malloc/malloc.h>
void *(*system_malloc)(malloc_zone_t *zone, size_t size);
void (*system_free)(malloc_zone_t *zone, void *ptr);
void *(*system_calloc)(malloc_zone_t *zone, size_t num_items, size_t size);
void (*system_free_ds)(malloc_zone_t *zone, void *ptr, size_t size);
static malloc_zone_t original_zone;


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

#elif __linux

#define MEMORY_CLEAN 0

extern void *__libc_malloc(size_t size);
extern void *__libc_calloc(size_t num, size_t size);
extern void __libc_free( void *ptr );

void *my_malloc_hook (size_t size, void *caller);
void *my_calloc_hook (size_t num, size_t size, void *caller);
void my_free_hook( void *ptr, void *caller );

int malloc_hook_active = 0;

void *malloc (size_t size) {
    void *caller = __builtin_return_address(0);
    if (malloc_hook_active)
        return my_malloc_hook(size, caller);
    return __libc_malloc(size);
}
void *calloc (size_t num, size_t size) {
    void *caller = __builtin_return_address(0);
    if (malloc_hook_active)
        return my_calloc_hook(num, size, caller);
    return __libc_calloc(num, size);
}

void free ( void *ptr ) {
    void *caller = __builtin_return_address(0);
    if (malloc_hook_active)
        my_free_hook(ptr, caller);
    else
        __libc_free(ptr);
}

void *my_malloc_hook (size_t size, void *caller) {
    void *ptr;
    malloc_hook_active = 0;
    ptr = malloc( size );
    malloc_hook_active = 1;

    memory_count += 1;

    return ptr;
}
void *my_calloc_hook (size_t num, size_t size, void *caller) {
    void *ptr;
    malloc_hook_active = 0;
    ptr = calloc( num, size );
    malloc_hook_active = 1;

    memory_count += 1;

    return ptr;
}
void my_free_hook ( void *ptr, void *caller ) {
    malloc_hook_active = 0;
    free( ptr );
    malloc_hook_active = 1;

    memory_count -= 1;
}

#elif __FreeBSD__
#include <dlfcn.h>
/*HOOKS IMPLEMENTATION*/
#define PARENT_FRAME 0x01

#define MEMORY_CLEAN 1

void *(*__malloc)(size_t size);
void *(*__malloc_hook)(size_t size, const void *caller);
void *(*__cur_malloc_hook)(size_t size, const void *caller);

void *(*__calloc)(size_t num, size_t size);
void *(*__calloc_hook)(size_t num, size_t size, const void *caller);
void *(*__cur_calloc_hook)(size_t num, size_t size, const void *caller);

void (*__free)(void *ptr);
void (*__free_hook)(void *ptr, const void *caller);
void (*__cur_free_hook)(void *ptr, const void *caller);

void *malloc(size_t size){
    const void *caller = __builtin_return_address(PARENT_FRAME);
    __malloc = dlsym(RTLD_NEXT, "malloc");

    if(__malloc_hook)
        return __malloc_hook(size, caller);

    return __malloc(size);
}

void *calloc(size_t num, size_t size) {
    const void *caller = __builtin_return_address(PARENT_FRAME);
    __calloc = dlsym(RTLD_NEXT, "calloc");

    if(__calloc_hook)
        return __calloc_hook(num, size, caller);

    return __calloc(num, size);
}

void free( void *ptr ) {
    const void *caller = __builtin_return_address(PARENT_FRAME);
    __free = dlsym(RTLD_NEXT, "free");

    if (__free_hook)
        __free_hook( ptr, caller );
    else
        __free( ptr );
}

/*IMPLEMENTATION ENDS AND USER APP CODE BEGINS*/

void *my_malloc_hook(size_t size, const void *caller){
    /*protecting our hook's printf from endless recursion*/
    __cur_malloc_hook = __malloc_hook;
    __malloc_hook = NULL;

    memory_count += 1;

    __malloc_hook = __cur_malloc_hook;
    return __malloc(size);
}

void *my_calloc_hook(size_t num, size_t size, const void *caller){
    /*protecting our hook's printf from endless recursion*/
    __cur_calloc_hook = __calloc_hook;
    __calloc_hook = NULL;

    memory_count += 1;

    __calloc_hook = __cur_calloc_hook;

    return __calloc(num, size);
}

void my_free_hook( void *ptr, const void *caller ) {
    __cur_free_hook = __free_hook;
    __free_hook = NULL;

    memory_count -= 1;

    __free_hook = __cur_free_hook;
    __free( ptr );
}

void my_init_hooks() {
    __malloc_hook = my_malloc_hook;
    __calloc_hook = my_calloc_hook;
    __free_hook = my_free_hook;
}

#endif

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

int main( int argc, char *argv[] ) {

    memory_count = 0;
#ifdef __APPLE__
    my_malloc_init();
#elif __linux
    malloc_hook_active = 0;
#elif __FreeBSD__
    my_init_hooks();
#endif

    BPTree *tree = BPTree_Init();
    BPTreeRecord *root = tree->root;

    int i;
    for ( i=0; i < 100; i++ ) {
        BPTreeRecord *head = BPTreeRecord_AddRecord( root );
        printf( "\rHead %d", i );

        if ( i%7 == 0 && i != 0 ) {
            BPTreeRecord *right_head = BPTreeRecord_InsertRecordAfter( head );
            add_childs( right_head, 6 );
        }
        add_childs( head, 100 );
    }
#if defined(__linux) || defined(__APPLE__)
    printf("\nMemory allocated: %llu\n", memory_count);
    BPTree_Final( tree );
    printf("Memory count after final: %llu\n", memory_count);
    if ( memory_count == MEMORY_CLEAN )
        printf("Memory is clean. OK!\n");
#elif __FreeBSD__
    printf("\nMemory allocated: %lu\n", memory_count);
    BPTree_Final( tree );
    printf("Memory count after final: %lu\n", memory_count);
    if ( memory_count == MEMORY_CLEAN )
        printf("Memory is clean. OK!\n");
#else
    BPTree_Final( tree );
    printf("\n");
#endif
    return 0;
}
