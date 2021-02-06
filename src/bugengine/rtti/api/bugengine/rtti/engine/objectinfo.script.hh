/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_OBJECTINFO_SCRIPT_HH_
#define BE_RTTI_ENGINE_OBJECTINFO_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/rtti/stdafx.h>
#include <bugengine/rtti/engine/helper/staticarray.hh>
#include <bugengine/rtti/value.hh>

namespace BugEngine { namespace RTTI {

struct Tag;

struct be_api(RTTI) ObjectInfo
{
    raw< const ObjectInfo > const         next;
    raw< const staticarray< const Tag > > tags;
    istring const                         name;
    mutable Value                         value;
};

}}  // namespace BugEngine::RTTI

/**************************************************************************************************/
#endif
