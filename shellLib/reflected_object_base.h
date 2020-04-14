#ifndef REFLECTED_VALUE_BASE_H
#define REFLECTED_VALUE_BASE_H


#include <string>
#include <vector>
#include <stdint.h>


class ReflectedObjectBase
{
public:
    ReflectedObjectBase() = default;

    virtual size_t Size() const = 0;
    virtual std::vector<std::string> Keys() const = 0;
    virtual std::string Get(const std::string & ID) const = 0;
    virtual std::string GetSafely(const std::string & ID) const = 0;
    virtual bool SetSafely(const std::string &ID, const std::string &val) = 0;
    virtual void Set(const std::string &ID, const std::string & value) = 0;
    virtual std::string GetType(const std::string & ID) const = 0;

};

#endif // REFLECTED_VALUE_BASE_H
