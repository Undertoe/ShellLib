#include "string_helpers.h"


std::vector<std::string> StringHelpers::split(std::string_view line, char delim)
{
    std::vector<std::string> retval;
    uint32_t last = 0;
    for(uint32_t i = 0; i < line.size() ; i ++)
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


int StringHelpers::count_delims(std::string_view line, char delim)
{
    int count = 0;
    for(auto & c : line)
    {
        if(c == delim)
        {
            count ++;
        }
    }
    return count;
}

std::string_view StringHelpers::find_between_delims(std::string_view line, char delim)
{
    auto first = line.find_first_of(delim);
    auto last = line.find_last_of(delim);
    return line.substr(first + 1, last - first - 1);
}
