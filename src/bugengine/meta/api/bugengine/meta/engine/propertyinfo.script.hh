/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_META_ENGINE_PROPERTYINFO_SCRIPT_HH_
#define BE_META_ENGINE_PROPERTYINFO_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/meta/stdafx.h>
#include <bugengine/meta/engine/helper/staticarray.hh>
#include <bugengine/meta/typeinfo.script.hh>

namespace BugEngine { namespace Meta {

struct Tag;
class Value;

struct be_api(META) Property
{
    friend class Value;
published:
    raw< const staticarray< const Tag > > tags;
    istring                               name;
    Type                                  owner;
    Type                                  type;

    Value get(Value & from) const;
    Value get(const Value& from) const;
    void  set(Value & from, const Value& value) const;

    Value getTag(const Type& tagType) const;
    Value getTag(raw< const Class > tagType) const;

public:
    Value (*getter)(void* data, bool isConst);
};

}}  // namespace BugEngine::Meta

/**************************************************************************************************/
#endif
