/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/stdafx.h>
#include <bugengine/script.script.hh>

namespace BugEngine {

Script::Script(weak< const File > file) : m_file(file)
{
}

Script::~Script()
{
}

ifilename Script::getScriptName() const
{
    return m_file->getFileName();
}

}  // namespace BugEngine
