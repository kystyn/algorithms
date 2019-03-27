#ifndef B_TREE_H
#define B_TREE_H

#include "def.h"

typedef struct tag_node {
    uint nodeCount;
    uint *keys;
    struct tag_node **children;
} node;

typedef struct tag_b_tree {
    uint degree;
    node *root;
} b_tree;

void initTree( b_tree *t, uint degree );
uint search( b_tree *t, uint key );
void insert( b_tree *t, uint key );
///FORTEST
uint yyy( uint a, uint b );

#endif // B_TREE_H
