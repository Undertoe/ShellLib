#include "string_helpers.h"


std::vector<std::string> StringHelpers::split(std::string_view line, char delim)
{
    std::vector<std::string> retval;
    int last = 0;
    for(int i = 0; i < line.size() ; i ++)
    {
        if(line[i] == delim)
        {
            retval.emplace_back(line.substr(last, i - last));
            last = i + 1;
        }
    }
    retval.emplace_back(line.substr(last, line.size() - last));
    return retval;
}
