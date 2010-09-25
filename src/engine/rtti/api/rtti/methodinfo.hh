/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_METHODINFO_HH_
#define BE_RTTI_METHODINFO_HH_
/*****************************************************************************/
#include   <rtti/typeinfo.hh>

namespace BugEngine { namespace RTTI
{

struct ParamInfo
{
    const char *const   name;
    const TypeInfo      type;
};

struct MethodInfo
{
    const char *const   name;
    const TypeInfo      returntype;
    const ParamInfo*    parameters;
};

}}

/*****************************************************************************/
#endif
