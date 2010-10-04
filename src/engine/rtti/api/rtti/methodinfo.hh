/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_METHODINFO_HH_
#define BE_RTTI_METHODINFO_HH_
/*****************************************************************************/
#include   <rtti/typeinfo.hh>
#include   <rtti/value.hh>

namespace BugEngine { namespace RTTI
{

struct ParamInfo
{
    const char *const   name;
    const TypeInfo      type;
};

struct OverloadInfo
{
    const TypeInfo      returntype;
    const size_t        paramCount;
    const ParamInfo*    parameters;
    Value               (*method)(Value* values, size_t params);
};

struct MethodInfo
{
    const char *const           name;
    const size_t                overloadCount;
    const OverloadInfo* const   overloads;
};

}}

/*****************************************************************************/
#endif
