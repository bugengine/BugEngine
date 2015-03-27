/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <pythonlib/stdafx.h>
#include    <pythonlib/pythonscript.script.hh>

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
