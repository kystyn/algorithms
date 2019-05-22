#include <stdio.h>
#include <string.h>
#include "double_hash.h"

uint hash( char *key, uint p ) {
    uint i, powered_p = 1, res = 0;

    for (i = 0; i < strlen(key); i++)
        res += (powered_p *= p) * key[i];

    return res;
}

uint hash1( char *key ) {
    return hash(key, 3);
}

uint hash2( char *key ) {
    return hash(key, 2);
}


int main()
{
    double_hash_table t;
    initHashTable(&t, 2, hash1, hash2);
    addToHashTable(&t, "abcd");
    addToHashTable(&t, "bcaca");
    addToHashTable(&t, "cerf");

    uint
            neok1 = searchHashTable(&t, "a"),
            ok1 = searchHashTable(&t, "bcaca"),
            ok2 = searchHashTable(&t, "cerf"),
            neok = searchHashTable(&t, "dasdasd"),
            ok3 = searchHashTable(&t, "abcd");

    freeHashTable(&t);

    printf("Hello World!\n");
    return 0;
}
