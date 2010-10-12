/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_PROPERTYINFO_HH_
#define BE_RTTI_PROPERTYINFO_HH_
/*****************************************************************************/
#include   <rtti/typeinfo.hh>

namespace BugEngine { namespace RTTI
{

struct PropertyInfo
{
    const char *        name;
    ClassInfo const*    type;
};

}}

/*****************************************************************************/
#endif
