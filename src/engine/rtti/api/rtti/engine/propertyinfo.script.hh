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

struct be_api(RTTI) PropertyInfo
{
    friend class BugEngine::Value;
published:
    TagInfo* const      tags;
    const PropertyInfo* next;
    TypeInfo            owner;
    TypeInfo            type;
    u32                 offset;

    Value get(Value& from) const;
    void  set(Value& from, const Value& value) const;

    Value getTag(const TypeInfo& type) const;
    Value getTag(const ClassInfo* type) const;
private:
    PropertyInfo& operator=(const PropertyInfo&);
};

}}

/*****************************************************************************/
#endif
