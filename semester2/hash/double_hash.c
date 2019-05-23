#include <stdlib.h>
#include <string.h>
#include "double_hash.h"

void initHashTable(
        double_hash_table *t, uint m,
        uint (*hash1)( const char *key ),
        uint (*hash2)( const char *key )) {
    t->hash1 = hash1;
    t->hash2 = hash2;
    t->m = m;
    t->table = malloc(sizeof(array) * m);

    uint idx;
    char *nil = NULL;

    for (idx = 0; idx < t->m; idx++) {
        InitArray(&t->table[idx], sizeof(const char *), 1);
        AddToArray(&t->table[idx], &nil);
    }
}

void addHashTable( double_hash_table *t, const char *key ) {
    uint i;
    uint minOccupancy = 0xFFFFFFFF, minOccupancyCell = 0;
    for (i = 0; i < t->m; i++) {
        char **elem = GetByIdx(&t->table[(t->hash1(key) + i * t->hash2(key)) % t->m], 0);
        if (*elem == NULL) {
            *elem = malloc(strlen(key) + 1);
            strcpy(*elem, key);
            break;
        }
        // find the least occupied cell
        else if (t->table[(t->hash1(key) + i * t->hash2(key)) % t->m].NumBlock < minOccupancy) {
            minOccupancyCell = (t->hash1(key) + i * t->hash2(key)) % t->m;
            minOccupancy = t->table[(t->hash1(key) + i * t->hash2(key)) % t->m].NumBlock;
        }
    }
    // all cells busy
    if (i == t->m) {
        char *elem = malloc(strlen(key) + 1);
        strcpy(elem, key);
        AddToArray(&t->table[minOccupancyCell], &elem);
    }
}

uint searchHashTable( double_hash_table *t, const char *key ) {
    uint i;

    for (i = 0; i < t->m; i++) {
        char **elem = GetByIdx(&t->table[(t->hash1(key) + i * t->hash2(key)) % t->m], 0);
        if (*elem != NULL) {
            uint j;

            for (j = 0; j < t->table[(t->hash1(key) + i * t->hash2(key)) % t->m].NumBlock; j++)
                if (*(char **)GetByIdx(&t->table[(t->hash1(key) + i * t->hash2(key)) % t->m], j) != NULL)
                    if (strcmp(*(char **)GetByIdx(&t->table[(t->hash1(key) + i * t->hash2(key)) % t->m], j), key) == 0)
                        return 1;
        }
    }

    return 0;
}

void deleteKeyHashTable( double_hash_table *t, const char *key ) {
  uint i;

    for (i = 0; i < t->m; i++) {
        char **elem = GetByIdx(&t->table[(t->hash1(key) + i * t->hash2(key)) % t->m], 0);
        if (*elem != NULL) {
            uint j;

            for (j = 0; j < t->table[(t->hash1(key) + i * t->hash2(key)) % t->m].NumBlock; j++)
                if (*(char **)GetByIdx(&t->table[(t->hash1(key) + i * t->hash2(key)) % t->m], j) != NULL)
                    if (strcmp(*(char **)GetByIdx(&t->table[(t->hash1(key) + i * t->hash2(key)) % t->m], j), key) == 0)
                        DeleteFromArray(&t->table[(t->hash1(key) + i * t->hash2(key)) % t->m], j);
        }
    }
}

void freeHashTable( double_hash_table *t ) {
    uint i, j;

    for (i = 0; i < t->m; i++) {
        for (j = 0; j < t->table[i].CurrentListSize; j++)
            if (*(char **)GetByIdx(&t->table[i], j) != NULL)
                free(*(char **)GetByIdx(&t->table[i], j));
        FreeArray(&t->table[i]);
    }
    free(t->table);
}
