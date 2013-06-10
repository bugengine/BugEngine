/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
 see LICENSE for detail */

#include    <filesystem/stdafx.h>
#include    <filesystem/diskfolder.script.hh>
#include    <filesystemwatch.hh>


namespace BugEngine
{

DiskFolder::Watch::Watch(weak<DiskFolder> folder)
    :   m_folder(folder)
{
}

DiskFolder::Watch::~Watch()
{
}

}
