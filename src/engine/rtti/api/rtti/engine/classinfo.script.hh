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
    friend struct TypeInfo;
public:
    raw<const char>         name;
    raw<const ClassInfo>    parent;
    raw<const ClassInfo>    metaclass;
    size_t                  size;
    size_t                  propertyCount;
    raw<PropertyInfo const> properties;
private:
    void copy(const void* src, void* dst) const;
    void destroy(void* src) const;
};

}}

/*****************************************************************************/
#endif
