#ifndef TST_SPLIT_H
#define TST_SPLIT_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>


#include "string_helpers.h"

using namespace testing;

TEST(stringhelpers, split)
{
    std::string test = "This is a test!";
    auto split = StringHelpers::split(test, ' ');
    std::cout << split.size() << std::endl;
    for(const auto & s : split)
    {
        std::cout << s << std::endl;
    }
}

#endif // TST_SPLIT_H
