#ifndef FUNCTION_POINTER_H
#define FUNCTION_POINTER_H

#include <string>

namespace FunctionPointers {

template<typename CallerType>
using funcptr_t = int(CallerType::*)(const std::string &);


struct FunctionPointerBase
{
    FunctionPointerBase() = default;


    virtual int Call(const std::string & args) = 0;

};


template<typename Caller>
class FunctionPointer : public FunctionPointerBase
{
private:
    Caller * _caller;
    funcptr_t<Caller> _func;

public:
    FunctionPointer(Caller * caller, funcptr_t<Caller> func) :
        FunctionPointerBase(),
        _caller(caller),
        _func(func)
    {

    }


    virtual int Call(const std::string &args) override
    {
        return(_caller->*_func)(args);
    }

};

}


#endif // FUNCTION_POINTER_H
