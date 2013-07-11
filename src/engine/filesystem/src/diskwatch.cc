/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
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