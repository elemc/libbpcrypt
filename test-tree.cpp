// -*- C++ -*-
/* ------------------------------------ */
/* C++ source source                    */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */

/* Name: (test-tree)
   Description: 
*/

#include <libbpcrypt/tree.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <list>

using namespace std;

list<BPTreeRecord *> ptrList;

void add_childs( BPTreeRecord *parent, unsigned int count ) {
    int j;
    for( j = 0; j < count; j++ ) {
        cout << "\r\t\tChild " << j << " of " <<  count;
        BPTreeRecord *child = (BPTreeRecord *)BPTreeRecord_AddRecord( parent );
        ptrList.push_back( child );

        if ( j%7 == 0 && j!=0 ) {
            BPTreeRecord *left_child = (BPTreeRecord *)BPTreeRecord_InsertRecordBefore( child );
            ptrList.push_back( left_child );
            add_childs( left_child, 6 );
        }
    }
}

int main( int argc, char *argv[] ) {

    BPTree *tree = (BPTree *)BPTree_Init();
    BPTreeRecord *root = tree->root;

    ptrList.push_back( root );

    int i, j;
    for ( i=0; i < 1000; i++ ) {
        BPTreeRecord *head = (BPTreeRecord *)BPTreeRecord_AddRecord( root );
        ptrList.push_back(head);
        cout << "\rHead " << i;

        if ( i%7 == 0 && i != 0 ) {
            BPTreeRecord *right_head = (BPTreeRecord *)BPTreeRecord_InsertRecordAfter( head );
            ptrList.push_back( right_head );
            add_childs( right_head, 6 );
        }
        add_childs( head, 1000 );
    }

    BPTree_Final( tree );
    //printf("\n");
    cout << "\n";

    // Check memory free
    unsigned long long checker = 0;
    list<BPTreeRecord *>::iterator it;
    for( it=ptrList.begin(); it != ptrList.end(); ++it ) {
        if ( *it != NULL )
            checker++;
    }
    if ( checker > 0 )
        cout << "Memory is unclean. " << checker << " pointers don\'t cleaned";


    return 0;
}
