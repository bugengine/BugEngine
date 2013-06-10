/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <filesystem/stdafx.h>
#include    <filesystemwatch.hh>

#include    <macosx/watch.hh>
#include    <macosx/watchpoint.hh>
#include    <macosx/watchqueue.hh>

namespace BugEngine
{

FileSystemWatch::FileSystemWatch()
    :   m_queue(scoped<FileSystemWatchProcessQueue>::create(BugEngine::Arena::filesystem()))
{
}

FileSystemWatch::~FileSystemWatch()
{
}

ref<DiskFolder::Watch> FileSystemWatch::watchDirectory(weak<DiskFolder> folder, const BugEngine::ipath& path)
{
    return s_fileSystemWatch.m_queue->addFolder(folder, path);
}

FileSystemWatch FileSystemWatch::s_fileSystemWatch;

}
