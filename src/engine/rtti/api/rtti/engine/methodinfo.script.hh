/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_METHODINFO_SCRIPT_HH_
#define BE_RTTI_ENGINE_METHODINFO_SCRIPT_HH_
/*****************************************************************************/
#include   <rtti/typeinfo.hh>
#include   <rtti/value.hh>

namespace BugEngine { namespace RTTI
{

class ParamInfo
{
published:
    istring const   name;
    TypeInfo const  type;
public:
    ParamInfo(const istring& name, const TypeInfo& type) : name(name), type(type) { }
};

class OverloadInfo
{
published:
    TypeInfo const              returnType;
    minitl::vector<ParamInfo>   params;
public:
    Value (*call)(Value* params, size_t nparams);

    OverloadInfo(const TypeInfo& returnType, Value (*call)(Value* params, size_t nparams))
        :   returnType(returnType)
        ,   params(rttiArena())
        ,   call(call)
    { }
};

class MethodInfo : public minitl::refcountable
{
published:
    minitl::vector<OverloadInfo>    overloads;

    MethodInfo();
published:
    void test(ref<const ClassInfo> p1, ref<const ClassInfo> p2);
public:
    Value call(Value* params, size_t nparams) const;
};

}}

/*****************************************************************************/
#endif
