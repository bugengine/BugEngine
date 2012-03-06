/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_METHODINFO_SCRIPT_HH_
#define BE_RTTI_ENGINE_METHODINFO_SCRIPT_HH_
/*****************************************************************************/
#include   <rtti/typeinfo.hh>
#include   <rtti/value.hh>

namespace BugEngine { namespace RTTI
{


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
    raw<const Method>   call;
    raw<const Method>   next;
    raw<const Overload> overloads;
published:
    Value doCall(Value* params, u32 nparams) const;
private:
    Method& operator=(const Method&);
};


}}

/*****************************************************************************/
#endif
