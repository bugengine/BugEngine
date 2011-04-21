/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_METHODINFO_SCRIPT_HH_
#define BE_RTTI_ENGINE_METHODINFO_SCRIPT_HH_
/*****************************************************************************/
#include   <rtti/typeinfo.hh>
#include   <rtti/value.hh>

namespace BugEngine { namespace RTTI
{

struct be_api(RTTI) ParamInfo
{
published:
    istring const   name;
    TypeInfo const  type;
public:
    ParamInfo(const istring& name, const TypeInfo& type) : name(name), type(type) { }
};

struct be_api(RTTI) OverloadInfo
{
published:
    TypeInfo const                  returnType;
    minitl::vector<const ParamInfo> params;
    bool                            vararg;
public:
    Value (*call)(Value* params, u32 nparams);

    OverloadInfo(const TypeInfo& returnType, Value (*call)(Value* params, u32 nparams), bool vararg = false)
        :   returnType(returnType)
        ,   params(rttiArena())
        ,   vararg(vararg)
        ,   call(call)
    { }
    u32 distance(Value* p, u32 nparams) const;
};

struct be_api(RTTI) MethodInfo
{
published:
    minitl::vector<OverloadInfo>  overloads;
published:
    Value operator()(Value* params, u32 nparams) const;
public:
    MethodInfo();
};

}}

/*****************************************************************************/
#endif
