#include <stdlib.h>
#include <string.h>
#include "double_hash.h"

void initHashTable(
        double_hash_table *t, uint m,
        uint (*hash1)( char *key ),
        uint (*hash2)( char *key )) {
    t->hash1 = hash1;
    t->hash2 = hash2;
    t->m = m;
    t->table = malloc(sizeof(array) * m);

    uint idx;

    for (idx = 0; idx < t->m; idx++) {
        InitArray(&t->table[idx], sizeof(uint), 1);
        *(char **)GetByIdx(&t->table[idx], 0) = NULL; // NULL-initialization
    }
}

void add( double_hash_table *t, char *key ) {
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

uint search( double_hash_table *t, char *key ) {
    uint i;

    for (i = 0; i < t->m; i++) {
        char **elem = GetByIdx(&t->table[(t->hash1(key) + i * t->hash2(key)) % t->m], 0);
        if (*elem != NULL) {
            uint j;

            for (j = 0; j < t->table[(t->hash1(key) + i * t->hash2(key)) % t->m].NumBlock; j++)
                if (strcmp(GetByIdx(&t->table[(t->hash1(key) + i * t->hash2(key)) % t->m], j), key) == 0)
                    return 1;
        }
    }

    return 0;
}
