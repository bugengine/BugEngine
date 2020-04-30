/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PACKAGE_PACKAGE_HH_
#define BE_PACKAGE_PACKAGE_HH_
/**************************************************************************************************/
#include <bugengine/plugin.scripting.package/stdafx.h>
#include <bugengine/core/endianness.hh>

namespace BugEngine { namespace PackageManager {

struct PackageHeader
{
    u32_l objectCount;
};

}}  // namespace BugEngine::PackageManager

/**************************************************************************************************/
#endif
