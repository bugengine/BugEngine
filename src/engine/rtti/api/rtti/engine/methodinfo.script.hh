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
    istring         name;
    TypeInfo const  type;
};

struct OverloadInfo
{
    TypeInfo const              returnType;
    minitl::vector<ParamInfo>   params;
};

class MethodInfo : public minitl::refcountable
{
public:
    minitl::vector<OverloadInfo>    overloads;

    MethodInfo();
};

}}

/*****************************************************************************/
#endif
