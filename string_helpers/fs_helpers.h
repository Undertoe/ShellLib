#ifndef FS_HELPERS_H
#define FS_HELPERS_H

//#include <iostream>
#include <string_view>
#include <experimental/filesystem>

namespace fs_helpers
{
    namespace fs = std::experimental::filesystem;

    bool AbsolutePath(std::string_view path);
    fs::path VerifyExtention(std::string_view filename, std::string_view extention);
    bool ContainedLocally(fs::path currentDir, std::string_view fileName);
    bool ContainedInLocalDir(fs::path currentDir, std::string_view fileName, std::string_view dirName);

};

#endif // FS_HELPERS_H
