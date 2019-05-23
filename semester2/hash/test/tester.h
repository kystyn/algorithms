#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

extern "C" {
#include "../double_hash.h"
}

using namespace testing;

uint hash( const char *key, uint p ) {
    uint i, powered_p = 1, res = 0;

    for (i = 0; i < strlen(key); i++)
        res += (powered_p *= p) * key[i];

    return res;
}

uint hash1( const char *key ) {
    return hash(key, 3);
}

uint hash2( const char *key ) {
    return hash(key, 2);
}


TEST(EasyHash, Search) {
    double_hash_table t;
    initHashTable(&t, 2, hash1, hash2);
    addHashTable(&t, "abcd");
    addHashTable(&t, "bcaca");
    addHashTable(&t, "cerf");

    ASSERT_FALSE(searchHashTable(&t, "a"));
    ASSERT_TRUE(searchHashTable(&t, "abcd"));
    ASSERT_TRUE(searchHashTable(&t, "bcaca"));
    ASSERT_TRUE(searchHashTable(&t, "cerf"));
    ASSERT_FALSE(searchHashTable(&t, "dasdasd"));

    freeHashTable(&t);
}

TEST(EasyHash, Delete) {
    double_hash_table t;
    initHashTable(&t, 2, hash1, hash2);
    addHashTable(&t, "abcd");
    deleteKeyHashTable(&t, "abcd");
    addHashTable(&t, "bcaca");
    deleteKeyHashTable(&t, "bcaca");
    addHashTable(&t, "cerf");

    ASSERT_FALSE(searchHashTable(&t, "a"));
    ASSERT_FALSE(searchHashTable(&t, "abcd"));
    ASSERT_FALSE(searchHashTable(&t, "bcaca"));
    ASSERT_TRUE(searchHashTable(&t, "cerf"));
    ASSERT_FALSE(searchHashTable(&t, "dasdasd"));

    freeHashTable(&t);
}

TEST(CollisedHash, Search) {
    double_hash_table t;
    initHashTable(&t, 2, hash1, hash2);
    addHashTable(&t, "abcd");
    addHashTable(&t, "bcaca");
    addHashTable(&t, "cerf");

    ASSERT_FALSE(searchHashTable(&t, "a"));
    ASSERT_TRUE(searchHashTable(&t, "abcd"));
    ASSERT_TRUE(searchHashTable(&t, "bcaca"));
    ASSERT_TRUE(searchHashTable(&t, "cerf"));
    ASSERT_FALSE(searchHashTable(&t, "dasdasd"));

    freeHashTable(&t);
}

TEST(CollisedHash, Delete) {
    double_hash_table t;
    initHashTable(&t, 2, hash1, hash2);
    addHashTable(&t, "abcdasdasdasd");
    addHashTable(&t, "bcacaqweqweqweqwe");
    addHashTable(&t, "cerfqweqweqweqeqeqeqeqe");

    deleteKeyHashTable(&t, "a");
    deleteKeyHashTable(&t, "abcdasdasdasd");
    ASSERT_FALSE(searchHashTable(&t, "abcdasdasdasd"));
    addHashTable(&t, "ab");
    deleteKeyHashTable(&t, "cerfqweqweqweqeqeqeqeqe");
    ASSERT_FALSE(searchHashTable(&t, "cerfqweqweqweqeqeqeqeqe"));
    addHashTable(&t, "cd");
    deleteKeyHashTable(&t, "dasdasd");
    ASSERT_FALSE(searchHashTable(&t, "dasdasd"));
    deleteKeyHashTable(&t, "bcacaqweqweqweqwe");
    ASSERT_FALSE(searchHashTable(&t, "bcacaqweqweqweqwe"));
    ASSERT_TRUE(searchHashTable(&t, "cd"));
    ASSERT_TRUE(searchHashTable(&t, "ab"));
    freeHashTable(&t);
}