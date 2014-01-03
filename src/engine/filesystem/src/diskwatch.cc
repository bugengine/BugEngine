/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <filesystem/stdafx.h>
#include    <filesystem/diskfolder.script.hh>
#include    <diskwatch.hh>
#include    <watchpoint.hh>

namespace BugEngine
{

DiskFolder::Watch::Watch(weak<DiskFolder> folder, weak<FileSystem::WatchPoint> watchPoint)
    :   Folder::Watch(folder)
    ,   m_watchPoint(watchPoint)
{
    m_watchPoint->addWatch(this);
}

DiskFolder::Watch::~Watch()
{
    m_watchPoint->removeWatch(this);
}

}
