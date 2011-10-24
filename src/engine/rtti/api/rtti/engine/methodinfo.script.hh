/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_METHODINFO_SCRIPT_HH_
#define BE_RTTI_ENGINE_METHODINFO_SCRIPT_HH_
/*****************************************************************************/
#include   <rtti/typeinfo.hh>
#include   <rtti/value.hh>

namespace BugEngine { namespace RTTI
{


struct be_api(RTTI) MethodInfo
{
published:
    struct be_api(RTTI) OverloadInfo
    {
    published:
        struct be_api(RTTI) ParamInfo
        {
        published:
            TagInfo* const          tags;
            const ParamInfo* const  next;
            istring                 name;
            TypeInfo                type;
        published:
            Value getTag(const TypeInfo& type) const;
            Value getTag(const ClassInfo* type) const;
        private:
            ParamInfo& operator=(const ParamInfo&);
        };
    published:
        TagInfo* const      tags;
        const OverloadInfo* next;
        TypeInfo            returnType;
        const ParamInfo*    params;
        bool                vararg;
    published:
        Value getTag(const TypeInfo& type) const;
        Value getTag(const ClassInfo* type) const;
    public:
        Value (*call)(Value* params, u32 nparams);
        u32 distance(Value* p, u32 nparams) const;
    private:
        OverloadInfo& operator=(const OverloadInfo&);
    };

published:
    istring             name;
    const MethodInfo*   next;
    const OverloadInfo* overloads;
published:
    Value operator()(Value* params, u32 nparams) const;
private:
    MethodInfo& operator=(const MethodInfo&);
};

}}

/*****************************************************************************/
#endif
