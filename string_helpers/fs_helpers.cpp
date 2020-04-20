#include "fs_helpers.h"

bool FSHelpers::absolute_path(std::string_view path)
{
    return fs::path(path).has_parent_path();
}


FSHelpers::fs::path FSHelpers::verify_extention(std::string_view fileName, std::string_view extention)
{
    fs::path path(fileName);
    if(path.extension() != extention)
    {
        path += extention;
    }

    return path;
}


bool FSHelpers::contained_locally(fs::path currentDir, std::string_view fileName)
{
    for(const auto & item : fs::directory_iterator(currentDir))
    {
        if(item.path().filename() == fileName)
        {
            return true;
        }
    }
    return false;
}

bool FSHelpers::contained_in_local_dir(fs::path currentDir, std::string_view fileName, std::string_view dirName)
{
    fs::path path = currentDir;
    path /= dirName;
    if(!fs::exists(path))
    {
        return false;
    }
    for(const auto & item : fs::directory_iterator(path))
    {
        if(item.path().filename() == fileName)
        {
            return true;
        }
    }
    return false;
}
