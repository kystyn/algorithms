#pragma once

#include <gtest/gtest.h>
//#include <gmock/gmock-matchers.h>

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
    return hash(key, 19);
}

uint hash2( const char *key ) {
    return hash(key, 7);
}

TEST(EasyHash, Add) {
  double_hash_table t;
  initHashTable(&t, 5, hash1, hash2);
  addKeyHashTable(&t, "abcd");
  addKeyHashTable(&t, "bcaca");
  addKeyHashTable(&t, "cerf");

  ASSERT_TRUE(strcmp(*(char **)(t.table.List + t.table.BlockSize * 0), "abcd") == 0);
  ASSERT_TRUE(strcmp(*(char **)(t.table.List + t.table.BlockSize * 1), "bcaca") == 0);
  ASSERT_TRUE(strcmp(*(char **)(t.table.List + t.table.BlockSize * 2), "cerf") == 0);

  freeHashTable(&t);
}

TEST(EasyHash, Search) {
    double_hash_table t;
    initHashTable(&t, 5, hash1, hash2);
    addKeyHashTable(&t, "abcd");
    addKeyHashTable(&t, "bcaca");
    addKeyHashTable(&t, "cerf");

    ASSERT_FALSE(searchHashTable(&t, "a"));
    ASSERT_TRUE(searchHashTable(&t, "abcd"));
    ASSERT_TRUE(searchHashTable(&t, "bcaca"));
    ASSERT_TRUE(searchHashTable(&t, "cerf"));
    ASSERT_FALSE(searchHashTable(&t, "dasdasd"));

    freeHashTable(&t);
}

TEST(EasyHash, Delete) {
    double_hash_table t;
    initHashTable(&t, 5, hash1, hash2);
    addKeyHashTable(&t, "abcd");
    deleteKeyHashTable(&t, "abcd");
    addKeyHashTable(&t, "bcaca");
    deleteKeyHashTable(&t, "bcaca");
    addKeyHashTable(&t, "cerf");
    deleteKeyHashTable(&t, "bcaca");

    ASSERT_FALSE(searchHashTable(&t, "a"));
    ASSERT_FALSE(searchHashTable(&t, "abcd"));
    ASSERT_FALSE(searchHashTable(&t, "bcaca"));
    ASSERT_TRUE(searchHashTable(&t, "cerf"));
    ASSERT_FALSE(searchHashTable(&t, "dasdasd"));

    freeHashTable(&t);
}

TEST(CollisedHash, Add) {
    double_hash_table t;
    initHashTable(&t, 2, hash1, hash2);
    addKeyHashTable(&t, "abcd");
    addKeyHashTable(&t, "bcaca");
    addKeyHashTable(&t, "cerf");
    addKeyHashTable(&t, "qfwqfqw");

    ASSERT_TRUE(strcmp(*(char **)(t.table.List + t.table.BlockSize * 0), "abcd") == 0);
    ASSERT_TRUE(strcmp(*(char **)(t.table.List + t.table.BlockSize * 1), "bcaca") == 0);
    ASSERT_TRUE(strcmp(*(char **)(t.table.List + t.table.BlockSize * 2), "cerf") == 0);
    ASSERT_TRUE(strcmp(*(char **)(t.table.List + t.table.BlockSize * 3), "qfwqfqw") == 0);

    freeHashTable(&t);
}

TEST(CollisedHash, Search) {
    double_hash_table t;
    initHashTable(&t, 2, hash1, hash2);
    addKeyHashTable(&t, "abcd");
    addKeyHashTable(&t, "bcaca");
    addKeyHashTable(&t, "cerf");
    addKeyHashTable(&t, "qfwqfqw");
    addKeyHashTable(&t, "skdvsdflkgj");
    addKeyHashTable(&t, "pwipowe");
    addKeyHashTable(&t, "abcd");

    ASSERT_FALSE(searchHashTable(&t, "a"));
    ASSERT_TRUE(searchHashTable(&t, "abcd"));
    ASSERT_TRUE(searchHashTable(&t, "bcaca"));
    ASSERT_TRUE(searchHashTable(&t, "cerf"));
    ASSERT_TRUE(searchHashTable(&t, "pwipowe"));
    ASSERT_TRUE(searchHashTable(&t, "skdvsdflkgj"));
    ASSERT_TRUE(searchHashTable(&t, "qfwqfqw"));
    ASSERT_FALSE(searchHashTable(&t, "dasdasd"));

    freeHashTable(&t);
}

TEST(CollisedHash, Delete) {
    double_hash_table t;
    initHashTable(&t, 2, hash1, hash2);
    addKeyHashTable(&t, "abcdasdasdasd");
    addKeyHashTable(&t, "bcacaqweqweqweqwe");
    addKeyHashTable(&t, "cerfqweqweqweqeqeqeqeqe");

    deleteKeyHashTable(&t, "a");
    deleteKeyHashTable(&t, "abcdasdasdasd");
    ASSERT_FALSE(searchHashTable(&t, "abcdasdasdasd"));
    addKeyHashTable(&t, "ab");
    deleteKeyHashTable(&t, "cerfqweqweqweqeqeqeqeqe");
    ASSERT_FALSE(searchHashTable(&t, "cerfqweqweqweqeqeqeqeqe"));
    addKeyHashTable(&t, "cd");
    deleteKeyHashTable(&t, "dasdasd");
    ASSERT_FALSE(searchHashTable(&t, "dasdasd"));
    deleteKeyHashTable(&t, "bcacaqweqweqweqwe");
    ASSERT_FALSE(searchHashTable(&t, "bcacaqweqweqweqwe"));
    ASSERT_TRUE(searchHashTable(&t, "cd"));
    ASSERT_TRUE(searchHashTable(&t, "ab"));

    freeHashTable(&t);
}

TEST(Tough, Add) {
  int i;
  static const int MaxSize = 100;
  double_hash_table t;
  initHashTable(&t, 5, hash1, hash2);
  char str[MaxSize + 1] = { 0 };
  for (i = 0; i < MaxSize; i++) {
    str[i] = 'a';
    addKeyHashTable(&t, str);
  }

  ASSERT_TRUE(searchHashTable(&t, str));
  for (i = MaxSize - 1; i > 0; i--) {
    str[i] = 0;
    ASSERT_TRUE(searchHashTable(&t, str));
  }

  freeHashTable(&t);
}

TEST(Tough, Delete) {

  int i;
  static const int MaxSize = 100;
  double_hash_table t;
  initHashTable(&t, 5, hash1, hash2);
  char str[MaxSize + 1] = { 0 };
  for (i = 0; i < MaxSize; i++) {
    str[i] = 'a';
    addKeyHashTable(&t, str);
  }

  deleteKeyHashTable(&t, str);
  for (i = MaxSize - 1; i > MaxSize / 2; i--) {
    str[i] = 0;
    deleteKeyHashTable(&t, str);
  }

  memset(str, 0, MaxSize + 1);
  for (i = 0; i < MaxSize / 2; i++) {
    str[i] = 'a';
    ASSERT_TRUE(searchHashTable(&t, str));
  }

  for (i = MaxSize / 2; i < MaxSize; i++) {
    str[i] = 'a';
    ASSERT_FALSE(searchHashTable(&t, str));
  }

  freeHashTable(&t);

}
