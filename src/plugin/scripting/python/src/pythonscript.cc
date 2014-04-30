/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <stdafx.h>
#include    <pythonscript.script.hh>

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
