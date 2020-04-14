#ifndef REFLECTED_FUNCTIONS_BASE_H
#define REFLECTED_FUNCTIONS_BASE_H

#include <string>
#include <vector>
#include <stdint.h>

namespace Terryn {


class FunctionPtrBase
{
public:
    FunctionPtrBase() = default;

    virtual int Call(const std::string & key, const std::string & args) = 0;
    virtual std::vector<std::string> Keys() const = 0;
    virtual size_t Size() const = 0;
};

}

#endif // REFLECTED_FUNCTIONS_BASE_H
