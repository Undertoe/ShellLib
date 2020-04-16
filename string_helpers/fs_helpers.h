#ifndef FS_HELPERS_H
#define FS_HELPERS_H

#include <string_view>
#include <experimental/filesystem>

namespace fs_helpers
{
    namespace fs = std::experimental::filesystem;

    bool AbsolutePath(std::string_view path);
    fs::path VerifyExtention(std::string_view filename, std::string_view extention);
    bool ContainedLocally(std::string_view fileName, std::string_view currentDir);
    bool ContainedInLocalDir(std::string_view fileName, std::string_view currentDir, std::string_view dirName);

};

#endif // FS_HELPERS_H
