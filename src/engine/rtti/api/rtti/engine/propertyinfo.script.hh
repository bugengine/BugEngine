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

struct Tag;

struct be_api(RTTI) Property
{
    friend class BugEngine::Value;
published:
    raw<Tag> const      tags;
    raw<const Property> next;
    const istring       name;
    Type                owner;
    Type                type;
    u32                 offset;

    Value get(Value& from) const;
    void  set(Value& from, const Value& value) const;

    Value getTag(const Type& type) const;
    Value getTag(raw<const Class> type) const;
private:
    Property& operator=(const Property&);
};

}}

/*****************************************************************************/
#endif
