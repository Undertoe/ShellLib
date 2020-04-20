#ifndef TST_SPLIT_H
#define TST_SPLIT_H

#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <experimental/filesystem>

#include "fs_helpers.h"
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


TEST(StringHelpers, trim_substr_front)
{
    std::string test = "Hello, \"This is a quoted string.\"";

    auto substr = StringHelpers::trim_substr_front(test, "Hello, ");


    EXPECT_TRUE(substr);
    EXPECT_EQ(*substr, "\"This is a quoted string.\"");
}

TEST(FS, file_system_checks)
{
    namespace fs = std::experimental::filesystem;
    fs::path checkPath = "/home/terryn/sandbox/testData/aliasTest.g4sh";
    fs::path anotherPath = "aliasTest.g4sh";


    std::cout << checkPath << ": " << std::boolalpha << checkPath.has_parent_path() << std::endl;
    std::cout << anotherPath << ": " << std::boolalpha << anotherPath.has_parent_path() << std::endl;

    EXPECT_TRUE(fs_helpers::AbsolutePath(checkPath.c_str()));
    EXPECT_EQ(fs_helpers::AbsolutePath(anotherPath.c_str()), false);
}

TEST(FS, verify_extention)
{
    namespace fs = std::experimental::filesystem;
    fs::path checkWithExtention = "aliasTest.g4sh";
    fs::path checkNoExtention = "aliasTest";

    EXPECT_EQ(checkWithExtention, fs_helpers::VerifyExtention(checkWithExtention.c_str(), ".g4sh"));
    EXPECT_EQ(checkWithExtention, fs_helpers::VerifyExtention(checkNoExtention.c_str(), ".g4sh"));
    std::cout << fs_helpers::VerifyExtention(checkNoExtention.c_str(), ".g4sh") << std::endl;
}


TEST(FS, contained_locally)
{
    namespace fs = std::experimental::filesystem;
    fs::path currentPath = "/home/terryn/sandbox/testData";
    std::string_view scriptsDir = "Scripts";

    std::string_view scriptsFile = "NEW.g4sh";
    std::string_view currentPathFile = "testScript.g4sh";

    EXPECT_TRUE(fs_helpers::ContainedLocally(currentPath, currentPathFile));
    fs::path extendedPath = currentPath;
    extendedPath /= scriptsDir;
    EXPECT_TRUE(fs_helpers::ContainedLocally(extendedPath, scriptsFile));
    EXPECT_TRUE(fs_helpers::ContainedInLocalDir(currentPath, scriptsFile, scriptsDir));

}

#endif // TST_SPLIT_H
