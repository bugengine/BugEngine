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

struct PropertyInfo
{
    raw<const char>         name;
    TypeInfo                type;
    Value                   (*get)(Value& object);
    void                    (*set)(Value& object, Value& value);
};

}}

/*****************************************************************************/
#endif
