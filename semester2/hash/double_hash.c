#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "double_hash.h"

uint mySearchHashTable( double_hash_table *t, const char *key, uint *idxFound );

void initHashTable(
        double_hash_table *t, uint m,
        uint (*hash1)( const char *key ),
        uint (*hash2)( const char *key )) {
    t->hash1 = hash1;
    t->hash2 = hash2;
    t->m = m;
    InitArray(&t->table, sizeof(char *), t->m);
}

uint easyAddHashTable( double_hash_table *t, const char *key, uint *idxFound ) {
    uint i;
    for (i = 0; i < t->m; i++) {
        const char **elem = GetByIdx(&t->table, (t->hash1(key) + i * t->hash2(key)) % t->m);
        if (*elem == NULL) {
            char *newEl = malloc(strlen(key) + 1);
            *idxFound = (t->hash1(key) + i * t->hash2(key)) % t->m;
            strcpy(newEl, key);
            AddToArray(&t->table, &newEl);
            return 1;
        }
    }

    if (i == t->m) {
        // check all cells
        for (i = 0; i < t->m; i++) {
            const char **elem = GetByIdx(&t->table, i);
            if (*elem == NULL) {
                *idxFound = i;
                char *newEl = malloc(strlen(key) + 1);
                strcpy(newEl, key);
                AddToArray(&t->table, &newEl);
                return 1;
            }
        }
        if (i == t->m)
            return 0;
    }
    else
        return 0;

    return 1;
}

void addKeyHashTable( double_hash_table *t, const char *key ) {
    char **tmp;
    uint idxFound;

    if (searchHashTable(t, key))
      return;

    if (!easyAddHashTable(t, key, &idxFound)) {
        tmp = malloc(sizeof(char *) * t->m);

        uint j;

        for (j = 0; j < t->m; j++) {
            char *elem = *(char **)GetByIdx(&t->table, j);
            if (elem != NULL)
                tmp[j] = elem;
        }
        FreeArray(&t->table);
        t->m <<= 1;
        InitArray(&t->table, sizeof(char *), t->m);

        for (j = 0; j < t->m / 2; j++)
            easyAddHashTable(t, tmp[j], &idxFound);
        if (!easyAddHashTable(t, key, &idxFound))
            printf("Something gonna wrong!\n");

        for (j = 0; j < t->m / 2; j++)
            if (tmp[j] != NULL)
                free(tmp[j]);

        free(tmp);
    }
}

uint mySearchHashTable( double_hash_table *t, const char *key, uint *idxFound ) {
    uint i;

    for (i = 0; i < t->m; i++) {
        const char **elem = GetByIdx(&t->table, (t->hash1(key) + i * t->hash2(key)) % t->m);
        if (*elem != NULL)
            if (strcmp(*elem, key) == 0) {
                *idxFound = (t->hash1(key) + i * t->hash2(key)) % t->m;
                return 1;
            }
    }
    for (i = 0; i < t->table.NumBlock; i++) {
        const char **elem = GetByIdx(&t->table, i);
        if (*elem != NULL)
            if (strcmp(*elem, key) == 0) {
                *idxFound = i;
                return 1;
            }
    }

    return 0;
}

uint searchHashTable( double_hash_table *t, const char *key )  {
  uint idx;
  return mySearchHashTable(t, key, &idx);
}

void deleteKeyHashTable( double_hash_table *t, const char *key ) {
    uint idxFound;

    if (mySearchHashTable(t, key, &idxFound)) {
        free(*(char **)GetByIdx(&t->table, idxFound));
        DeleteFromArray(&t->table, idxFound);
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
