#ifndef FUNCTION_CONTROL_H
#define FUNCTION_CONTROL_H

#include "function_control_base.h"
#include "reflected_function_collection.h"

template<class ... Types>
class FunctionControl : public FunctionControlBase
{
private:
    Terryn::ReflectedFunctionCollection<Types...> _funcs;

public:
    FunctionControl(Types* ... vals) :
        FunctionControlBase(),
        _funcs(vals...)
    {
        _funcs.CompleteInitialization();
    }

    virtual std::vector<std::string> Keys() const override
    {
        return _funcs.Keys();
    }


    virtual int Call(const std::string &key, const std::string &args) override
    {
        return _funcs.Call(key, args);
    }

};

#endif // FUNCTION_CONTROL_H
