#ifndef VARIABLE_CONTROL_BASE_H
#define VARIABLE_CONTROL_BASE_H


#include <string>
#include <vector>
#include <optional>

class VariableControlBase
{
public:

    VariableControlBase() = default;

    virtual std::vector<std::string> Keys() const = 0;
    virtual std::optional<std::string> Get(const std::string &ID) const = 0;
    virtual bool AssignValue(const std::string & ID, const std::string & value) = 0;
};

#endif // VARIABLE_CONTROL_BASE_H
