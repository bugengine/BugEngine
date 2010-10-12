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
    const char *        name;
    const ClassInfo*    type;
};

struct OverloadInfo
{
    const ClassInfo*    returntype;
    size_t              paramCount;
    const ParamInfo*    parameters;
    Value               (*method)(Value* params, size_t paramCount);
};

struct MethodInfo
{
    const char *        name;
    size_t              overloadCount;
    const OverloadInfo* overloads;
};

}}

/*****************************************************************************/
#endif
