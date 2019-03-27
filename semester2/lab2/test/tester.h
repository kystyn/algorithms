#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "../b_tree.h"

using namespace testing;

TEST(AddToBTree, Test1)
{
    EXPECT_EQ(search(NULL, 1), 0);
    ASSERT_THAT(0, Eq(0));
}
