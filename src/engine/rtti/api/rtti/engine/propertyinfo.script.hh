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

class PropertyInfo : public minitl::refcountable
{
public:
    const istring   name;
    const TypeInfo  type;
    PropertyInfo(const istring& name, const TypeInfo& type);
    ~PropertyInfo();
public:
    Value                   (*get)(Value& object);
    void                    (*set)(Value& object, Value& value);
};

}}

/*****************************************************************************/
#endif
