/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <python/stdafx.h>
#include    <python/pythonscript.script.hh>

namespace BugEngine
{

PythonScript::PythonScript(weak<const File> file)
    :   Script(file)
{
}

PythonScript::~PythonScript()
{
}

}
