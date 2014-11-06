// -*- C -*-
/* ------------------------------------ */
/* C source header                      */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */

/* Name: (xml)
   Description: 
*/

#ifndef XML_H
#define XML_H

#include "misc.h"
#include "tree.h"
#include <libxml/parser.h>
#include <libxml/tree.h>

#ifdef __cplusplus
extern "C"{
#endif

#ifdef WITH_OLD_CRYPT
extern BPTree *BP_xml_old_load_file( const char *filename, bp_buffer_t *key, bp_size_t key_size );
#endif

extern BPTree *BP_xml_load_file( const char *filename, bp_buffer_t *key, bp_size_t key_size );

#ifdef __cplusplus
}
#endif

#endif

