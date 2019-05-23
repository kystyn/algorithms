#include <stdio.h>
#include <string.h>
#include "double_hash.h"

#ifdef _WIN32
/* Memory leak checker */
#ifdef _DEBUG
# include <crtdbg.h>
# define SetDbgMemHooks() \
  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
  _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#endif /* _DEBUG */
#endif


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
#ifdef _WIN32
#ifdef   _DEBUG
  SetDbgMemHooks();
#endif
  #endif
    double_hash_table t;
    initHashTable(&t, 2, hash1, hash2);
    addHashTable(&t, "abcdasdasdasd");
    addHashTable(&t, "bcacaqweqweqweqwe");
    addHashTable(&t, "cerfqweqweqweqeqeqeqeqe");

    uint
            neok1 = searchHashTable(&t, "a"),
            ok1 = searchHashTable(&t, "abcdasdasdasd"),
            ok2 = searchHashTable(&t, "cerfqweqweqweqeqeqeqeqe"),
            neok = searchHashTable(&t, "dasdasd"),
            ok3 = searchHashTable(&t, "bcacaqweqweqweqwe");

    deleteKeyHashTable(&t, "a");
    deleteKeyHashTable(&t, "abcdasdasdasd");
    ok1 = searchHashTable(&t, "abcdasdasdasd"),
    addHashTable(&t, "ab");
    deleteKeyHashTable(&t, "cerfqweqweqweqeqeqeqeqe");
    addHashTable(&t, "cd");
    deleteKeyHashTable(&t, "dasdasd");
    deleteKeyHashTable(&t, "bcacaqweqweqweqwe");
    freeHashTable(&t);

    printf("Hello World!\n");
    return 0;
}
