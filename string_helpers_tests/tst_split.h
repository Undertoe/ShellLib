#ifndef TST_SPLIT_H
#define TST_SPLIT_H

#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "string_helpers.h"

using namespace testing;

TEST(StringHelpers, split)
{    
    std::string test = "Hello this is five strings";
    auto split = StringHelpers::split(test, ' ');


    EXPECT_EQ(split.size(), 5);
    EXPECT_EQ(split[0], "Hello");
    EXPECT_EQ(split[1], "this");
    EXPECT_EQ(split[2], "is");
    EXPECT_EQ(split[3], "five");
    EXPECT_EQ(split[4], "strings");
}

#endif // TST_SPLIT_H
