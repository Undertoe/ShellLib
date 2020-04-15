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


    EXPECT_EQ(split.size(), 5u);
    EXPECT_EQ(split[0], "Hello");
    EXPECT_EQ(split[1], "this");
    EXPECT_EQ(split[2], "is");
    EXPECT_EQ(split[3], "five");
    EXPECT_EQ(split[4], "strings");
}

TEST(StringHelpers, count_delims)
{
    std::string test = "Hello, \"This is a quoted string.\"";

    auto count = StringHelpers::count_delims(test, '\"');
    EXPECT_EQ(count, 2);
}

TEST(StringHelpers, find_between_delims)
{

    std::string test = "Hello, \"This is a quoted string.\"";

    auto substr = StringHelpers::find_between_delims(test, '\"');

    EXPECT_EQ(std::string(substr), "This is a quoted string.");
    std::cout << substr << std::endl;
}

#endif // TST_SPLIT_H
