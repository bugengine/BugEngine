
/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
 see LICENSE for detail */

#include <bugengine/filesystem/stdafx.h>
#include <bugengine/filesystem/folder.script.hh>

namespace BugEngine {

Folder::Watch::Watch(weak< Folder > folder) : m_folder(folder)
{
}

Folder::Watch::~Watch()
{
}

void Folder::Watch::signal()
{
    m_folder->onChanged();
}

}  // namespace BugEngine
