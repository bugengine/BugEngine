/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/plugin.scripting.package/stdafx.h>
#include <bugengine/plugin.scripting.package/package.script.hh>

namespace BugEngine {

Package::Package(weak< const File > file) : Script(file)
{
}

Package::~Package()
{
}

}  // namespace BugEngine
