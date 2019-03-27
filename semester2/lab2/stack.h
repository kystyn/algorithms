#ifndef STACK_H
#define STACK_H

#include "def.h"
#include "array.h"

typedef array stack;

void initStack( stack *s, uint dataSize );
void push( stack *s, void *what );
void * pop( stack *s );
void * top( stack *s );

#endif // STACK_H
