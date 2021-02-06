/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_FILESYSTEM_WATCHQUEUE_HH_
#define BE_FILESYSTEM_WATCHQUEUE_HH_
/**************************************************************************************************/
#include <bugengine/filesystem/stdafx.h>
#include <bugengine/core/threads/thread.hh>
#include <bugengine/filesystem/diskfolder.script.hh>
#include <bugengine/kernel/interlocked.hh>
#include <watchpoint.hh>

#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>

namespace BugEngine { namespace FileSystem {

class FileSystemWatchProcessQueue : public minitl::pointer
{
private:
    CFStringRef          m_Path;
    Thread               m_thread;
    iptr< CFRunLoopRef > m_runLoop;

private:
    static intptr_t runFileSystemWatch(intptr_t p1, intptr_t p2);
    static void     onFileSystemEvent(ConstFSEventStreamRef streamRef, void* clientCallBackInfo,
                                      size_t numEvents, void* eventPaths,
                                      const FSEventStreamEventFlags eventFlags[],
                                      const FSEventStreamEventId    eventIds[]);

public:
    FileSystemWatchProcessQueue();
    ~FileSystemWatchProcessQueue();

    ref< DiskFolder::Watch > addFolder(weak< DiskFolder > folder, const BugEngine::ipath& path);
};

}}  // namespace BugEngine::FileSystem

/**************************************************************************************************/
#endif
