#include "fs_helpers.h"

bool fs_helpers::AbsolutePath(std::string_view path)
{
    return fs::path(path).has_parent_path();
}


fs_helpers::fs::path fs_helpers::VerifyExtention(std::string_view fileName, std::string_view extention)
{
    fs::path path(fileName);
    if(path.extension() != extention)
    {
        path += extention;
    }

    return path;
}


bool fs_helpers::ContainedLocally(std::string_view fileName, std::string_view currentDir)
{
    fs::path tmp(currentDir);
    for(const auto & item : fs::directory_iterator(tmp))
    {
        if(item == fileName)
        {
            return true;
        }
    }
    return false;
}

bool fs_helpers::ContainedInLocalDir(std::string_view fileName, std::string_view currentDir, std::string_view dirName)
{
    fs::path path = currentDir;
    path /= dirName;
    if(!fs::exists(path))
    {
        return false;
    }
    for(const auto & item : fs::directory_iterator(path))
    {
        if(item == fileName)
        {
            return true;
        }
    }
    return false;
}
