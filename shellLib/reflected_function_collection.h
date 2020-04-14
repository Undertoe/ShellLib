#ifndef SHELLLIB_H
#define SHELLLIB_H

#include <vector>
#include <map>
#include <memory>

#include "reflected_functions_base.h"
#include "reflected_functions.hh"

namespace Terryn {


template<class ... Types>
class ReflectedFunctionCollection
{
private:
    std::vector<std::unique_ptr<FunctionPtrBase>> _functions;
    std::vector<std::string> _keys;

    std::map<std::string, FunctionPtrBase*> _functionMapping;


public:

    template<class LastType>
    ReflectedFunctionCollection(LastType *n)
    {
        _functions.push_back(std::make_unique<FunctionPointer<LastType>>(n));
    }

    template<class CurrentType, class ... Rest>
    ReflectedFunctionCollection(CurrentType *c, Rest* ... r) :
        ReflectedFunctionCollection(r...)
    {
        _functions.push_back(std::make_unique<FunctionPointer<CurrentType>>(c));
    }

    void CompleteInitialization()
    {
        for(const auto & f : _functions)
        {
            auto keys = f->Keys();
            for(const auto & k : keys)
            {
                _keys.push_back(k);
                _functionMapping[k] = f.get();
            }
        }
    }


    std::vector<std::string> Keys() const
    {
        return _keys;
    }

    int Call(const std::string &key, const std::string &args)
    {
        for(auto & [k, func] : _functionMapping)
        {
            if(k == key)
            {
                return func->Call(key, args);
            }
        }
        return -1;
    }

};

}

#endif // SHELLLIB_H
