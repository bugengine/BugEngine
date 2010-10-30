/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_CLASSINFO_SCRIPT_HH_
#define BE_RTTI_ENGINE_CLASSINFO_SCRIPT_HH_
/*****************************************************************************/

namespace BugEngine
{
struct TypeInfo;
}

namespace BugEngine { namespace RTTI
{

struct PropertyInfo;
struct MethodInfo;

struct ClassInfo
{
    friend struct BugEngine::TypeInfo;
public:
    raw<const char>                 name;
    raw<const ClassInfo>            parent;
    raw<const ClassInfo>            metaclass;
    mutable raw<const ClassInfo>    child;
    mutable raw<const ClassInfo>    next;
    u16                             size;
    u16                             propertyCount;
    raw<PropertyInfo const>         properties;
private:
    void copy(const void* src, void* dst) const;
    void destroy(void* src) const;
};

}}

/*****************************************************************************/
#endif
