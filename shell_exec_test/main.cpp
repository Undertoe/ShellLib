#include <iostream>

#include "refl.hh"
#include "reflected_function_collection.h"
#include "reflected_object_collection.h"


#include <memory>
#include "variable_control.h"
#include "function_control.h"


using namespace std;

struct Struc1
{
    Struc1() = default;

    int a = 1;
    int b = 2;

    int Print1(const std::string & args)
    {
        std::cout << "1" << std::endl;
        return 0;
    }

    int Print2(const std::string & args)
    {
        std::cout << "2" << std::endl;
        return 0;
    }
};

struct Struc2
{
    Struc2() = default;

    int c = 3;
    float d = 4.4;
    int Print3(const std::string & args)
    {
        std::cout << "3" << std::endl;
        return 0;
    }

    int Print4(const std::string & args)
    {
        std::cout << "4 " << args << std::endl;
        return 0;
    }
};

struct Struc3
{

    Struc3() = default;

    double five = 5.5;
    double six = 6.6;
    int Print5(const std::string & args)
    {
        std::cout << "5" << std::endl;
        return 0;
    }

    int Print6(const std::string & args)
    {
        std::cout << "6 " << args << std::endl;
        return 0;
    }
};


REFL_AUTO(
        type(Struc1),
        field(a),
        field(b),
        func(Print1),
        func(Print2)
        )

REFL_AUTO(
        type(Struc2),
        field(c),
        field(d),
        func(Print3),
        func(Print4)
        )

REFL_AUTO(
        type(Struc3),
        field(five),
        field(six),
        func(Print5),
        func(Print6)
        )


int main()
{

    Struc1 one;
    Struc2 two;
    Struc3 three;

    Terryn::ReflectedFunctionCollection lib(&one, &two, &three);
    lib.CompleteInitialization();

    Terryn::ReflectedObjectCollection objs(&one, &two, &three);
    objs.CompleteInitialization();


    auto keys = lib.Keys();
    std::cout << "Size: " << lib.Keys().size() << std::endl;
    for(const auto & k : keys)
    {
        std::cout << "\t" << k << std::endl;
    }

    auto retVal = lib.Call("Print1", "unused args");
    std::cout << "Return Value: " << retVal << std::endl;

    retVal = lib.Call("Print4", "these are used args");
    std::cout << "Return Value: " << retVal << std::endl;

    retVal = lib.Call("Print0", "This is an uncalled function");
    std::cout << "Return Value: " << retVal << std::endl;
    if(retVal == -1)
    {
        std::cout << "Function was uncalled" << std::endl;
    }


    keys = objs.Keys();

    std::cout << "Size: " << objs.Keys().size() << std::endl;
    for(const auto & k : keys)
    {
        std::cout << "\t" << k << std::endl;
    }
    std::string key = "five";
    std::cout << "Acquiring: " << key << " = " << *objs.Get(key) << std::endl;
    std::cout << "Setting: " << key << " to 6.66" << std::endl;
    if(!objs.AssignValue(key, "6.66"))
    {
        std::cout << "Set failed!" << std::endl;
    }
    else
    {
        std::cout << "Set successful!" << std::endl;
    }
    std::cout << "Acquiring: " << key << " = " << *objs.Get(key) << std::endl;


    VariableControl vars(&one, &two, &three);

    keys = vars.Keys();

    std::cout << "Size: " << vars.Keys().size() << std::endl;
    for(const auto & k : keys)
    {
        std::cout << "\t" << k << std::endl;
    }
    std::cout << "Acquiring: " << key << " = " << *vars.Get(key) << std::endl;
    key = "six";
    std::cout << "Acquiring: " << key << " = " << *vars.Get(key) << std::endl;
    std::cout << "Setting: " << key << " to 7.77" << std::endl;
    if(!vars.AssignValue(key, "7.77"))
    {
        std::cout << "Set failed!" << std::endl;
    }
    else
    {
        std::cout << "Set successful!" << std::endl;
    }
    std::cout << "Acquiring: " << key << " = " << *vars.Get(key) << std::endl;


//    VariableControl* tmpControl = new VariableControl(&one, &two, &three);
//    std::unique_ptr<VariableControlBase> varControl = std::make_unique<VariableControl>(&one, &two, &three);


    return 0;
}
