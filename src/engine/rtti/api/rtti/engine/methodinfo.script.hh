/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_METHODINFO_SCRIPT_HH_
#define BE_RTTI_ENGINE_METHODINFO_SCRIPT_HH_
/**************************************************************************************************/
#include    <rtti/stdafx.h>
#include    <rtti/typeinfo.hh>
#include    <rtti/engine/helper/staticarray.hh>

namespace BugEngine { namespace RTTI
{

class Value;
struct Tag;

struct be_api(RTTI) Method
{
published:
    struct be_api(RTTI) Parameter
    {
    published:
        raw< staticarray<const Tag> >   tags;
        istring                         name;
        Type                            type;
    published:
        Value getTag(const Type& type) const;
        Value getTag(raw<const Class> type) const;
    private:
        Parameter& operator=(const Parameter&);
    };
    struct be_api(RTTI) Overload
    {
    published:
        raw< staticarray<const Tag> >       tags;
        raw< staticarray<const Parameter> > params;
        Type                                returnType;
        bool                                vararg;
    published:
        Value getTag(const Type& tagType) const;
        Value getTag(raw<const Class> tagType) const;
        u32 parameterCount() const { return params ? params->count : 0; }
    public:
        Value (*call)(Value* params, u32 nparams);
        u32 distance(Value* p, u32 paramCount) const;
    private:
        Overload& operator=(const Overload&);
    };

published:
    istring                             name;
    raw< staticarray<const Overload> >  overloads;
    be_tag(Alias("?call"))
    raw<const Method>                   call;
published:
    Value doCall(Value* params, u32 nparams) const;
private:
    Method& operator=(const Method&);
};

}}

/**************************************************************************************************/
#endif
