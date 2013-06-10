/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_FILESYSTEM_FILESYSTEMWATCH_HH_
#define BE_FILESYSTEM_FILESYSTEMWATCH_HH_
/*****************************************************************************/
#include    <filesystem/diskfolder.script.hh>

namespace BugEngine
{

class FileSystemWatch
{
private:
    class FileSystemWatchProcessQueue;
    scoped<FileSystemWatchProcessQueue> m_queue;
private:
    FileSystemWatch();
    ~FileSystemWatch();

    static BE_EXPORT FileSystemWatch s_fileSystemWatch;
public:
    static ref<DiskFolder::Watch> watchDirectory(weak<DiskFolder> folder, const ipath& path);
};

}

/*****************************************************************************/
#endif
