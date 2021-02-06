/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_PACKAGE_PACKAGE_SCRIPT_HH_
#define BE_PACKAGE_PACKAGE_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/plugin.scripting.package/stdafx.h>
#include <bugengine/rtti/tags/editor.script.hh>
#include <bugengine/script.script.hh>

namespace BugEngine {

class PackageLoader;

class be_api(PACKAGE) Package : public Script
{
    friend class PackageLoader;
published:
    Package(weak< const File > file);
    ~Package();
};

}  // namespace BugEngine

/**************************************************************************************************/
#endif
