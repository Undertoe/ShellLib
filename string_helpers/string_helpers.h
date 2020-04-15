#ifndef STRING_HELPERS_H
#define STRING_HELPERS_H

#include <string>
#include <vector>
#include <iostream>
#include <string_view>

namespace StringHelpers
{

    std::vector<std::string> split(std::string_view line, char delim);
    int count_delims(std::string_view line, char delim);
    std::string_view find_between_delims(std::string_view line, char delim);

}

#endif // STRING_HELPERS_H
