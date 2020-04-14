#ifndef REFLECTED_OBJECT_COLLECTION_H
#define REFLECTED_OBJECT_COLLECTION_H

#include <vector>
#include <map>
#include <memory>


#include "reflected_object_base.h"
#include "reflected_object.hh"

namespace Terryn {

template<class ... Types>
class ReflectedObjectCollection
{
private:
    std::vector<std::unique_ptr<ReflectedObjectBase>> _objects;
    std::vector<std::string> _keys;

    std::map<std::string, ReflectedObjectBase *>_objectMapping;

public:

    template<class LastType>
    ReflectedObjectCollection(LastType *n)
    {
        _objects.push_back(std::make_unique<ReflectedObject<LastType>>(n));
    }

    template<class CurrentType, class ... Rest>
    ReflectedObjectCollection(CurrentType * c, Rest * ... r) :
        ReflectedObjectCollection(r...)
    {
        _objects.push_back(std::make_unique<ReflectedObject<CurrentType>>(c));
    }

    void CompleteInitialization()
    {
        for(const auto & o : _objects)
        {
            auto keys = o->Keys();
            for(const auto & key : keys)
            {
                _keys.push_back(key);
                _objectMapping[key] = o.get();
            }
        }
    }

    std::vector<std::string> Keys() const
    {
        return _keys;
    }

    std::optional<std::string> Get(const std::string & ID) const
    {
        for(const auto & [key, obj] : _objectMapping)
        {
            if(key == ID)
            {
                return obj->Get(ID);
            }
        }

        return std::nullopt;
    }


    bool AssignValue(const std::string &ID, const std::string & val)
    {
        for(const auto & [key, obj] : _objectMapping)
        {
            if(key == ID)
            {
                obj->Set(ID, val);
                return true;
            }
        }
        return false;
    }


};


}


#endif // REFLECTED_OBJECT_COLLECTION_H
