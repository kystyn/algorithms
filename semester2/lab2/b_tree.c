#include <stdlib.h>
#include <string.h>

#include "b_tree.h"
#include "stack.h"

void initNode( b_tree *t, node *n, uint size ) {
    if (size == 0) {
        n->keys = NULL;
        n->children = NULL;
    }
    else {
        n->keys = (uint *)malloc(sizeof(int) * (2 * t->degree - 1));
        n->children = (node **)malloc(sizeof(node *) * (2 * t->degree));
    }

    n->nodeCount = 0;
}

void initTree( b_tree *t, uint degree ) {
    if (t == NULL)
        return;
    t->degree = degree;
    initNode(t, t->root, 0);
}

uint search( b_tree *t, uint key ) {
    uint i, needPop = 1;
    stack s;
    node *n;

    if (t == NULL)
        return 0;

    n = t->root;

    initStack(&s, sizeof(node *));

    push(&s, n);

    while (s.NumBlock != 0) {
        needPop = 1;
        n = *(node **)top(&s);

        for (i = 0; i < n->nodeCount; i++)
            if (key == n->keys[i]) {
                FreeArray(&s);
                return 1;
            }

        if (n == NULL) {
            pop(&s);
            continue;
        }

        for (i = 0; i < n->nodeCount + 1; i++)
            if (key >= n->children[i]->keys[0] &&
                key <= n->children[i]->keys[n->children[i]->nodeCount - 1]) {
                push(&s, n->children[i]);

                needPop = 0;
                break;
            }

        if (needPop)
            pop(&s);
    }

    return 0;
}

void insert( b_tree *t, uint key ) {
    node
        *n, *parent;
    uint i;
    uint needPop = 1;
    stack s;

    struct tagdata{
        node *n, *parent;
    } data;

    initStack(&s, sizeof(data));

    data.n = t->root;
    data.parent = NULL;

    push(&s, &data);

    while (s.NumBlock != 0) {
        uint getOut = 0;

        needPop = 1;

        data = *(struct tagdata *)top(&s);
        n = data.n;
        parent = data.parent;

        if (n == NULL) {
            pop(&s);
            continue;
        }

        for (i = 0; i < n->nodeCount + 1; i++)
            if (key >= n->children[i]->keys[0] &&
                key <= n->children[i]->keys[n->children[i]->nodeCount - 1]) {
                data.parent = n;
                data.n = n->children[i];

                if (n->children == NULL)
                    getOut = 1;

                push(&s, &data);
                needPop = 0;

                break;
            }
        if (needPop)
            pop(&s);

        if (getOut)
            break;
    }

    // backtrack
    while (s.NumBlock != 0) {
        data = *(struct tagdata *)top(&s);
        n = data.n;
        parent = data.parent;

        // has a place to insert
        if (n->nodeCount < 2 * t->degree - 1) {
            uint numToInsert;
            // find place to insert
            for (numToInsert = 0; numToInsert < n->nodeCount; numToInsert++)
                if (n->keys[numToInsert] <= key && key <= n->keys[numToInsert + 1])
                    break; // insert after keys[numToInsert]

            for (i = n->nodeCount; i > numToInsert + 1; i--)
                n->keys[i] = n->keys[i - 1];

            n->keys[numToInsert + 1] = key;
            FreeArray(&s);
            return;
        }
        else if (n == t->root) {
            node *prevRoot = t->root;

            prevRoot->nodeCount /= 2;
            initNode(t, t->root, prevRoot->nodeCount);
            t->root->children[0] = prevRoot;

            prevRoot->children += prevRoot->nodeCount + 1;
            t->root->children[1] = prevRoot;

            t->root->nodeCount = 2;
            pop(&s);
        }
        else {
            i = n->nodeCount;
            // shift children
            for (i = n->nodeCount; parent->children[i - 1] != n; i--)
                parent->children[i] = parent->children[i - 1];
            // first (degree) - 1 to current
            n->nodeCount = t->degree - 1;
            // second (degree) - 1 to the following
            memcpy(parent->children + i, n->children + t->degree, sizeof(node *) * (t->degree - 1));
            pop(&s);
        }
    }
}