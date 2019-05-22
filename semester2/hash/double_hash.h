#ifndef DOUBLE_HASH_H
#define DOUBLE_HASH_H

#include "array.h"

typedef struct {
    uint (*hash1)( char *key );
    uint (*hash2)( char *key );
    uint m;
    array *table;
} double_hash_table;

// m - table size
void initHashTable(
        double_hash_table *t, uint m,
        uint (*hash1)( char *key ),
        uint (*hash2)( char *key ));

void addHashTable( double_hash_table *t, char *key );

// true - exists, false otherwise
uint searchHashTable( double_hash_table *t, char *key );

void deleteKeyHashTable( double_hash_table *t, char *key );

void freeHashTable( double_hash_table *t );

#endif // DOUBLE_HASH_H
