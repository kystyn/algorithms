#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

extern "C" {
#include "../double_hash.h"
}

using namespace testing;

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


TEST(EasyHash, Search) {
    double_hash_table t;
    initHashTable(&t, 2, hash1, hash2);
    addToHashTable(&t, "abcd");
    addToHashTable(&t, "bcaca");
    addToHashTable(&t, "cerf");

    ASSERT_FALSE(searchHashTable(&t, "a"));
    ASSERT_TRUE(searchHashTable(&t, "abcd"));
    ASSERT_TRUE(searchHashTable(&t, "bcaca"));
    ASSERT_TRUE(searchHashTable(&t, "cerf"));
    ASSERT_FALSE(searchHashTable(&t, "dasdasd"));

    freeHashTable(&t);
}

TEST(CollisedHash, Search) {
    double_hash_table t;
    initHashTable(&t, 2, hash1, hash2);
    addToHashTable(&t, "abcd");
    addToHashTable(&t, "bcaca");
    addToHashTable(&t, "cerf");

    ASSERT_FALSE(searchHashTable(&t, "a"));
    ASSERT_TRUE(searchHashTable(&t, "abcd"));
    ASSERT_TRUE(searchHashTable(&t, "bcaca"));
    ASSERT_TRUE(searchHashTable(&t, "cerf"));
    ASSERT_FALSE(searchHashTable(&t, "dasdasd"));

    freeHashTable(&t);
}
