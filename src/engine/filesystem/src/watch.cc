
/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
 see LICENSE for detail */

#include    <filesystem/stdafx.h>
#include    <filesystem/folder.script.hh>


namespace BugEngine
{

Folder::Watch::Watch(weak<Folder> folder)
    :   m_folder(folder)
{
}

Folder::Watch::~Watch()
{
}

void Folder::Watch::signal()
{
    m_folder->onChanged();
}

}
