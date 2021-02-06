/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_PROPERTYINFO_SCRIPT_HH_
#define BE_RTTI_ENGINE_PROPERTYINFO_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/rtti/stdafx.h>
#include <bugengine/rtti/engine/helper/staticarray.hh>
#include <bugengine/rtti/typeinfo.script.hh>

namespace BugEngine { namespace RTTI {

struct Tag;
class Value;

struct be_api(RTTI) Property
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

}}  // namespace BugEngine::RTTI

/**************************************************************************************************/
#endif
