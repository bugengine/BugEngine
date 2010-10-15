/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_CLASSINFO_SCRIPT_HH_
#define BE_RTTI_ENGINE_CLASSINFO_SCRIPT_HH_
/*****************************************************************************/

namespace BugEngine { namespace RTTI
{

struct PropertyInfo;
struct MethodInfo;

struct ClassInfo
{
    const char *            name;
    raw<const ClassInfo>    parent;
    raw<const ClassInfo>    metaclass;
    size_t                  size;

};

}}

/*****************************************************************************/
#endif
