/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <filesystem/stdafx.h>
#include    <filesystemwatch.hh>

namespace BugEngine
{

class FileSystemWatch::FileSystemWatchProcessQueue : public minitl::pointer
{
};

FileSystemWatch::FileSystemWatch()
    :   m_queue(scoped<FileSystemWatchProcessQueue>::create(BugEngine::Arena::filesystem()))
{
}

FileSystemWatch::~FileSystemWatch()
{
}

ref<DiskFolder::Watch> FileSystemWatch::watchDirectory(weak<DiskFolder> /*folder*/, const BugEngine::ipath& /*path*/)
{
    return ref<DiskFolder::Watch>();
}

FileSystemWatch FileSystemWatch::s_fileSystemWatch;

}
