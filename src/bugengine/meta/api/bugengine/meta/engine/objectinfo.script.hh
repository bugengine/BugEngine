/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_META_ENGINE_OBJECTINFO_SCRIPT_HH_
#define BE_META_ENGINE_OBJECTINFO_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/meta/stdafx.h>
#include <bugengine/meta/engine/helper/staticarray.hh>
#include <bugengine/meta/value.hh>

namespace BugEngine { namespace Meta {

struct Tag;

struct be_api(META) ObjectInfo
{
    raw< const ObjectInfo > const         next;
    raw< const staticarray< const Tag > > tags;
    istring const                         name;
    mutable Value                         value;
};

}}  // namespace BugEngine::Meta

/**************************************************************************************************/
#endif
