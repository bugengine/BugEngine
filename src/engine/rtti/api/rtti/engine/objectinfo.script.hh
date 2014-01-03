/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_OBJECTINFO_SCRIPT_HH_
#define BE_RTTI_ENGINE_OBJECTINFO_SCRIPT_HH_
/*****************************************************************************/
#include    <rtti/value.hh>

namespace BugEngine { namespace RTTI
{

struct Tag;

struct be_api(RTTI) ObjectInfo
{
    raw<const ObjectInfo> const     next;
    raw<Tag>                        tags;
    istring const                   name;
    Value                           value;
};

}}

/*****************************************************************************/
#endif

