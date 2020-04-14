#ifndef FUNCTION_CONTROL_BASE_H
#define FUNCTION_CONTROL_BASE_H


#include <string>
#include <vector>

class FunctionControlBase
{
public:

    FunctionControlBase() = default;
    virtual std::vector<std::string> Keys() const = 0;
    virtual int Call(const std::string & key, const std::string & args) = 0;
};


#endif // FUNCTION_CONTROL_BASE_H
