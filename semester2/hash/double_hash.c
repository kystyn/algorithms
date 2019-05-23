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
    InitArray(&t->table, sizeof(char *), t->m);
}

uint easyAddHashTable( double_hash_table *t, const char *key ) {
    uint i;
    for (i = 0; i < t->m; i++) {
        char **elem = GetByIdx(&t->table, (t->hash1(key) + i * t->hash2(key)) % t->m);
        if (*elem == NULL) {
            *elem = malloc(strlen(key) + 1);
            strcpy(*elem, key);
            AddToArray(&t->table, elem);
            break;
        }
    }
    if (i == t->m) {
        for (i = 0; i < t->m; i++) {
            char **elem = GetByIdx(&t->table, i);
            if (*elem == NULL) {
                *elem = malloc(strlen(key) + 1);
                strcpy(*elem, key);
                AddToArray(&t->table, elem);
                break;
            }
            if (i == t->m)
                return 0;
        }
    }
    return 1;
}

void addHashTable( double_hash_table *t, const char *key ) {
    array tmp;
    if (!easyAddHashTable(t, key)) {
        InitArray(&tmp, sizeof(char *), t->m);

        uint j;

        for (j = 0; j < t->m; j++) {
            char **elem = (char **)GetByIdx(&t->table, j);
            if (*elem != NULL)
                AddToArray(&tmp, elem);
        }
        FreeArray(&t->table);
        t->m <<= 1;
        InitArray(&t->table, sizeof(char *), t->m);

        for (j = 0; j < tmp.NumBlock; j++)
            easyAddHashTable(t, *(char **)GetByIdx(&tmp, j));
        easyAddHashTable(t, key);
        FreeArray(&tmp);
    }
}

uint searchHashTable( double_hash_table *t, const char *key ) {
    uint i;

    for (i = 0; i < t->m; i++) {
        char **elem = GetByIdx(&t->table, (t->hash1(key) + i * t->hash2(key)) % t->m);
        if (*elem != NULL) {
            if (strcmp(*elem, key) == 0)
                return 1;
        }
    }
    for (i = 0; i < t->m; i++) {
        char **elem = GetByIdx(&t->table, i);
        if (*elem != NULL) {
            if (strcmp(*elem, key) == 0)
                return 1;
        }
    }

    return 0;
}

void deleteKeyHashTable( double_hash_table *t, const char *key ) {
  uint i;

    for (i = 0; i < t->m; i++) {
        char **elem = GetByIdx(&t->table, (t->hash1(key) + i * t->hash2(key)) % t->m);
        if (*elem != NULL) {
            if (strcmp(*elem, key) == 0) {
                DeleteFromArray(&t->table, (t->hash1(key) + i * t->hash2(key)) % t->m);
                *elem = 0;
            }
        }
    }
}

void freeHashTable( double_hash_table *t ) {
    uint i;

    for (i = 0; i < t->m; i++) {
        char *elem = *(char **)GetByIdx(&t->table, i);
        if (elem != NULL)
            free(elem);
    }
    FreeArray(&t->table);
}
