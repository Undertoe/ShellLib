#ifndef REFLECTED_LAYERED_OBJECT_HH
#define REFLECTED_LAYERED_OBJECT_HH

#include <string>
#include <map>
#include <iostream>


#include "reflected_value.hh"
#include "reflected_object_base.h"

namespace Terryn {


template<typename ReflectedStructure>
class ReflectedObject : public ReflectedObjectBase
{
private:

    std::map<std::string, ReferencedType*> _ptrMap;
    std::vector<std::string> _keys;

    int32_t calls = 0;
    using MemoryMap = std::map<std::string, ReferencedType*>;


protected:

    template<typename SubStructure>
    MemoryMap HandleReflectedField(SubStructure* field)
    {
        MemoryMap retval;
        for_each(refl::reflect(*field).members, [&](auto member)
        {
            if constexpr (is_field(member))
            {
                auto mem = &member(*field);

                std::string name = get_display_name(member);
                auto nextMap = HandleReflectedField(mem);
                if(nextMap.size() > 0)
                {
                    for(auto &[mappedName, refType] : nextMap)
                    {
                        std::string newName = name + "." + mappedName;
                        retval[newName] = new ReferencedType(refType);
                    }
                }
                else
                {
                    if constexpr(AllowedType<decltype (mem)>())
                    {
                        retval[name] = new ReferencedType();
                        retval[name]->Set(mem);
                    }
                }
            }
        });

        return retval;

    }


public:
    ReflectedObject(ReflectedStructure *p) :
        ReflectedObjectBase()
    {
        _ptrMap = HandleReflectedField(p);
        _keys.reserve(_ptrMap.size());

        for(auto & [name, val] : _ptrMap)
        {
            _keys.emplace_back(name);
        }
    }


    virtual size_t Size() const override
    {
        return _ptrMap.size();
    }


    virtual std::vector<std::string> Keys() const override
    {
        return _keys;
    }

    virtual std::string Get(const std::string &ID) const override
    {
        return _ptrMap.at(ID)->Get();
    }

    virtual std::string GetSafely(const std::string &ID) const override
    {
        for(auto & [name, v] : _ptrMap)
        {
            if(name == ID)
            {
                return v->Get();
            }
        }
        return "Value: " + ID + " not present";
    }

    virtual bool SetSafely(const std::string &ID, const std::string &val) override
    {
        for(auto & [name, v]: _ptrMap)
        {
            if(name == ID)
            {
                v->ExternalAssign(val);
                return true;
            }
        }
        return false;
    }

    virtual void Set(const std::string &ID, const std::string &val) override
    {
        _ptrMap[ID]->ExternalAssign(val);
    }

    std::vector<size_t> Addresses()
    {
        std::vector<size_t> addresses;
        for (auto & [name, val]: _ptrMap)
        {
            addresses.push_back(val->Address());
        }
        return addresses;
    }

    std::map<std::string, size_t> AddressMap()
    {
        std::map<std::string, size_t> addrmap;
        for(auto & [name, val]: _ptrMap)
        {
            addrmap[name] = val->Address();
        }

        return addrmap;
    }

    /// performs opperations safely since this will never be a time critical event
    virtual std::string GetType(const std::string &ID) const override
    {
        for(auto & [name, v] : _ptrMap)
        {
            if(name == ID)
            {
                return v->GetType();
            }
        }
        return "Value: " + ID + " not present";
    }


};


}

#endif // REFLECTED_LAYERED_OBJECT_HH
