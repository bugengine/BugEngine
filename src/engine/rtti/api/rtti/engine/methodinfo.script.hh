/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_METHODINFO_SCRIPT_HH_
#define BE_RTTI_ENGINE_METHODINFO_SCRIPT_HH_
/**************************************************************************************************/
#include    <rtti/stdafx.h>
#include    <rtti/typeinfo.hh>

namespace BugEngine { namespace RTTI
{

class Value;
struct Tag;

struct be_api(RTTI) Method
{
published:
    struct be_api(RTTI) Overload
    {
    published:
        struct be_api(RTTI) Parameter
        {
        published:
            raw<Tag> const              tags;
            raw<const Parameter> const  next;
            istring                     name;
            Type                        type;
        published:
            Value getTag(const Type& type) const;
            Value getTag(raw<const Class> type) const;
        private:
            Parameter& operator=(const Parameter&);
        };
    published:
        raw<Tag> const          tags;
        raw<const Overload>     next;
        Type                    returnType;
        u32                     parameterCount;
        raw<const Parameter>    params;
        bool                    vararg;
    published:
        Value getTag(const Type& type) const;
        Value getTag(raw<const Class> type) const;
    public:
        Value (*call)(Value* params, u32 nparams);
        u32 distance(Value* p, u32 nparams) const;
    private:
        Overload& operator=(const Overload&);
    };

published:
    istring             name;
    raw<const Method>   next;
    be_tag(Alias("?call"))
    raw<const Method>   call;
    u32                 overloadCount;
    raw<const Overload> overloads;
published:
    Value doCall(Value* params, u32 nparams) const;
private:
    Method& operator=(const Method&);
};


}}

/**************************************************************************************************/
#endif
