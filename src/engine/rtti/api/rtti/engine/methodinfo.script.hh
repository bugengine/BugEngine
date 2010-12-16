/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_METHODINFO_SCRIPT_HH_
#define BE_RTTI_ENGINE_METHODINFO_SCRIPT_HH_
/*****************************************************************************/
#include   <rtti/typeinfo.hh>
#include   <rtti/value.hh>

namespace BugEngine { namespace RTTI
{

struct ParamInfo
{
    raw<const char>         name;
    raw<const ClassInfo>    type;
};

struct OverloadInfo
{
    raw<const ClassInfo>    returntype;
    u16                     paramCount;
    raw<const ParamInfo>    parameters;
    Value                   (*method)(Value* params, size_t paramCount);
};

class MethodInfo : public minitl::refcountable
{
public:
    const istring   name;

    Value operator()(Value* values, u32 paramCount);
};

}}

/*****************************************************************************/
#endif
