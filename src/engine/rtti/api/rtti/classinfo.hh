/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_CLASSINFO_HH_
#define BE_RTTI_CLASSINFO_HH_
/*****************************************************************************/

namespace BugEngine { namespace RTTI
{

struct PropertyInfo;
struct MethodInfo;

struct ClassInfo
{
    const char *        name;
    const ClassInfo*    parent;
    size_t              size;
    size_t              propertyCount;
    const PropertyInfo* properties;
    size_t              methodCount;
    MethodInfo*         methods;
    size_t              constructorCount;
    MethodInfo*         constructors;
    MethodInfo*         destructor;
    MethodInfo*         copy;
};

}}

/*****************************************************************************/
#endif
