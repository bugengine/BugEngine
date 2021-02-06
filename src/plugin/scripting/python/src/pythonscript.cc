/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.scripting.python/stdafx.h>
#include <bugengine/plugin.scripting.python/pythonscript.script.hh>

namespace BugEngine {

PythonScript::PythonScript(weak< const File > file) : Script(file)
{
}

PythonScript::~PythonScript()
{
}

}  // namespace BugEngine
