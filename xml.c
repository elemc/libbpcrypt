// -*- C -*-
/* ------------------------------------ */
/* C source source                      */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */

/* Name: (xml)
   Description: 
*/

#include "xml.h"

#ifdef WITH_OLD_CRYPT
#include "old_crypt.h"

void old_node_childs( xmlNode *node, BPTreeRecord *parent );

bp_buffer_t *old_get_record_name( xmlNode *node )
{
    xmlAttr *attr = node->properties;
    while( attr ) {
        if ( xmlStrcmp(attr->name, (const xmlChar *)"name") == 0 )
            return attr->children->content;
        attr = attr->next;
    }
    return NULL;
}

bp_buffer_t *old_get_record_text( xmlNode *node )
{
    xmlNode *child = node->children;
    while( child ) {
        if ( child->type == XML_TEXT_NODE )
            return child->content;
        child = child->next;
    }
    return NULL;
}

void old_node_item( xmlNode *node, BPTreeRecord *parent )
{
    BPTreeRecord *item_record = BPTreeRecord_AddRecord( parent );    
    bp_buffer_t *item_name = old_get_record_name( node );

    bp_buffer_t *login = NULL;
    bp_buffer_t *password = NULL;

    xmlNode *child = node->children;
    while( child ) {
        if ( xmlStrcmp( child->name, (const xmlChar *)"login" ) == 0 )
            login = old_get_record_text( child );
        else if ( xmlStrcmp( child->name, (const xmlChar *)"pwd" ) == 0 )
            password = old_get_record_text( child );
        child = child->next;
    }

    BPRecord_SetPassword( item_record, item_name, login, password );
}

void old_node_node( xmlNode *node, BPTreeRecord *parent )
{
    if ( xmlStrcmp( node->name, (const xmlChar *)"folder") == 0 ) {
        BPTreeRecord *fold = BPTreeRecord_AddRecord( parent );
        BPRecord_SetFold( fold, old_get_record_name( node ) );
        old_node_childs( node, fold );
    }
    else if ( xmlStrcmp( node->name, (const xmlChar *)"item") == 0 ) {
        old_node_item( node, parent );
    }
}

void old_node_childs( xmlNode *node, BPTreeRecord *parent )
{
    xmlNode *child_node = node->children;

    while( child_node ) {
        if ( child_node->type == XML_ELEMENT_NODE )
            old_node_node( child_node, parent );
        child_node = child_node->next;
    }
}

// load file with old crypt to tree
BPTree *BP_xml_old_load_file( const char *filename, bp_buffer_t *key, bp_size_t key_size )
{
#ifdef LIBXML_TREE_ENABLED
    LIBXML_TEST_VERSION
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

    bp_size_t crypt_data_size;
    bp_buffer_t *crypt_data = BP_read_file( filename, &crypt_data_size );
    if ( crypt_data == NULL ) {
        error( 501, 501, "Error loading data from file \"%s\" (%d)", filename, errno );
        return NULL;
    }

    bp_size_t decrypt_data_size;
    bp_buffer_t *decrypt_data = old_decrypt_buffer( crypt_data, crypt_data_size, key, key_size, &decrypt_data_size );
    if ( decrypt_data == NULL ) {
        free( crypt_data );
        error( 502, 502, "Error old decrypt data from file \"%s\" (%d)", filename, errno );
        return NULL;
    }
    free( crypt_data );

    doc = xmlReadMemory( decrypt_data, decrypt_data_size, NULL, NULL, 0 );
    if ( doc == NULL ) {
        error( 503, 503, "Error xml parse decrypt data from file \"%s\" (%d)", filename, errno );
        return NULL;
    }
    
    root_element = xmlDocGetRootElement(doc);

    BPTree *bp_tree = BPTree_Init(); 
    BPTreeRecord *bp_root = bp_tree->root;
    BPRecord_SetType( bp_root, BP_RECORD_ROOT );
    BPRecord_SetName( bp_root, (bp_buffer_t *)root_element->name ); 

    xmlFreeDoc(doc);
    xmlCleanupParser();

    free( decrypt_data );

    return bp_tree;
#else
    return NULL;
#endif
}
#endif

// load file with new crypt to tree
BPTree *BP_xml_load_file( const char *filename, bp_buffer_t *key, bp_size_t key_size )
{

}
