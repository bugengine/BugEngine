/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <filesystem/stdafx.h>
#include    <macosx/watch.hh>
#include    <macosx/watchpoint.hh>

namespace BugEngine { namespace FileSystem
{

DiskWatch::DiskWatch(weak<DiskFolder> folder, weak<WatchPoint> watchPoint)
    :   DiskFolder::Watch(folder)
    ,   m_watchPoint(watchPoint)
{
    m_watchPoint->addWatch(this);
}

DiskWatch::~DiskWatch()
{
    m_watchPoint->removeWatch(this);
}

}}