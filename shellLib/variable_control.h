#ifndef VARIABLECONTROL_H
#define VARIABLECONTROL_H



#include "reflected_object_collection.h"
#include "variable_control_base.h"

template<class ... Types>
class VariableControl : public VariableControlBase
{
private:

    Terryn::ReflectedObjectCollection<Types...> _objects;


public:

    VariableControl(Types* ... variables) :
        VariableControlBase(),
        _objects(variables...)
    {
        _objects.CompleteInitialization();
    }


    std::vector<std::string> Keys() const
    {
        return _objects.Keys();
    }

    std::optional<std::string> Get(const std::string &ID) const
    {
        return _objects.Get(ID);
    }

    bool AssignValue(const std::string & ID, const std::string & value)
    {
        return _objects.AssignValue(ID, value);
    }

};

#endif // VARIABLECONTROL_H
