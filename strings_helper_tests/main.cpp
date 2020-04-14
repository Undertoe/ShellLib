#include "tst_split.h"

#include <gtest/gtest.h>

int main(int argc, char *argv[])
{
    std::cout << "CHECK" << std::endl;
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
