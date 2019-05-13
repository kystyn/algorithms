#include <stdlib.h>
#include "stack.h"

void initStack( stack *s, uint dataSize ) {
    InitArray(s, dataSize);
}

void push( stack *s, void *what ) {
    AddToArray(s, what);
}

void * pop( stack *s ) {
    if (s->NumBlock == 0)
        return NULL;
    return GetByIdx(s, s->NumBlock-- - 1);
}

void * top( stack *s ) {
    return GetByIdx(s, s->NumBlock - 1);
}
