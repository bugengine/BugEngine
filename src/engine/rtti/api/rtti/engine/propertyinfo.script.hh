/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_PROPERTYINFO_SCRIPT_HH_
#define BE_RTTI_ENGINE_PROPERTYINFO_SCRIPT_HH_
/*****************************************************************************/
#include   <rtti/typeinfo.script.hh>

namespace BugEngine
{
class Value;
}

namespace BugEngine { namespace RTTI
{

class be_api(RTTI) PropertyInfo : public minitl::refcountable
{
public:
    enum PropertyFlags
    {
        Get,
        Set,
        GetSet
    };
public:
    const TypeInfo      type;
    const PropertyFlags flags;
public:
    PropertyInfo(const TypeInfo& type, PropertyFlags flags = GetSet);
    ~PropertyInfo();

    virtual Value get(Value& object) const;
    virtual void  set(Value& object, Value& value) const;
};

}}

/*****************************************************************************/
#endif
