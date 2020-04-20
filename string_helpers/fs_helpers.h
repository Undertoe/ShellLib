#ifndef FS_HELPERS_H
#define FS_HELPERS_H

//#include <iostream>
#include <string_view>
#include <experimental/filesystem>

namespace FSHelpers
{
    namespace fs = std::experimental::filesystem;

    bool absolute_path(std::string_view path);
    fs::path verify_extention(std::string_view filename, std::string_view extention);
    bool contained_locally(fs::path currentDir, std::string_view fileName);
    bool contained_in_local_dir(fs::path currentDir, std::string_view fileName, std::string_view dirName);

};

#endif // FS_HELPERS_H
