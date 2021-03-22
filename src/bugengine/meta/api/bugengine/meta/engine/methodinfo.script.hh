/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_META_ENGINE_METHODINFO_SCRIPT_HH_
#define BE_META_ENGINE_METHODINFO_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/meta/stdafx.h>
#include <bugengine/meta/engine/helper/staticarray.hh>
#include <bugengine/meta/typeinfo.hh>

namespace BugEngine { namespace Meta {

class Value;
struct Tag;

struct be_api(META) Method
{
published:
    struct be_api(META) Parameter
    {
    published:
        raw< const staticarray< const Tag > > tags;
        istring                               name;
        Type                                  type;
        raw< const Value >                    defaultValue;
    published:
        Value getTag(const Type& type) const;
        Value getTag(raw< const Class > type) const;

    public:
        static const Value s_noDefaultValue;
    };
    struct be_api(META) Overload
    {
    published:
        raw< const staticarray< const Tag > > tags;
        staticarray< const Parameter >        params;
        Type                                  returnType;
        bool                                  vararg;
    published:
        Value getTag(const Type& tagType) const;
        Value getTag(raw< const Class > tagType) const;

    public:
        staticarray< const Value > extraParameters;

    public:
        minitl::format< 1024u > signature() const;
        Value (*call)(Value * params, u32 nparams);
    };

published:
    istring                                    name;
    staticarray< const Overload >              overloads;
    be_tag(Alias("?call")) raw< const Method > call;

published:
    Value doCall(Value * params, u32 nparams) const;
};

}}  // namespace BugEngine::Meta

/**************************************************************************************************/
#endif
