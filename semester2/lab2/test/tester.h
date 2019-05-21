#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
extern "C" {
#include "../b_tree.h"
}

using namespace testing;

TEST(AddToBTree, Test1)
{
    EXPECT_EQ(search(nullptr, 1), 0);
    ASSERT_THAT(0, Eq(0));
    new int[100];
}
