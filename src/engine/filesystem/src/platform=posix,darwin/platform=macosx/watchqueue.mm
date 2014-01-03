/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <filesystem/stdafx.h>
#include    <macosx/watchqueue.hh>

#include    <watchpoint.hh>

#include    <core/environment.hh>
#include    <Foundation/Foundation.h>

namespace BugEngine { namespace FileSystem
{

static CFStringRef getApplicationDirectory()
{
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    NSString* bundlePath = [[NSBundle mainBundle] bundlePath];
    CFStringRef result = CFStringCreateWithCString(kCFAllocatorDefault, [bundlePath UTF8String], kCFStringEncodingUTF8);
    [pool release];
    return result;
}

FileSystemWatchProcessQueue::FileSystemWatchProcessQueue()
    :   m_Path(getApplicationDirectory())
    ,   m_thread("FileSystemWatch", &FileSystemWatchProcessQueue::runFileSystemWatch, (intptr_t)this, 0)
    ,   m_runLoop(0)
{
}

FileSystemWatchProcessQueue::~FileSystemWatchProcessQueue()
{
    while (!m_runLoop)
    {
        Thread::yield();
    }
    CFRunLoopStop(*m_runLoop);
    CFRelease(*m_runLoop);
    CFRelease(m_Path);
    m_thread.wait();
}

intptr_t FileSystemWatchProcessQueue::runFileSystemWatch(intptr_t p1, intptr_t /*p2*/)
{
    FileSystemWatchProcessQueue* watch = reinterpret_cast<FileSystemWatchProcessQueue*>(p1);
    CFRunLoopRef runLoop = CFRunLoopGetCurrent();
    CFRetain(runLoop);
    watch->m_runLoop = &runLoop;

    FSEventStreamContext context = { 0, (void*)watch, NULL, NULL, NULL };
    FSEventStreamRef eventStream = FSEventStreamCreate(kCFAllocatorDefault, &FileSystemWatchProcessQueue::onFileSystemEvent, &context, CFArrayCreate(kCFAllocatorDefault, (const void**)&watch->m_Path, 1, 0), kFSEventStreamEventIdSinceNow, 3.0, kFSEventStreamCreateFlagNone);
    FSEventStreamScheduleWithRunLoop(eventStream, runLoop, kCFRunLoopCommonModes);
    FSEventStreamStart(eventStream);

    CFRunLoopRun();

    FSEventStreamStop(eventStream);
    FSEventStreamInvalidate(eventStream);
    FSEventStreamRelease(eventStream);

    return 0;
}

void FileSystemWatchProcessQueue::onFileSystemEvent(ConstFSEventStreamRef /*streamRef*/,
                                                    void* /*clientCallBackInfo*/,
                                                    size_t eventCount,
                                                    void* eventPaths,
                                                    const FSEventStreamEventFlags /*eventFlags*/[],
                                                    const FSEventStreamEventId /*eventIds*/[])
{
    const char** eventPathsC = (const char**)eventPaths;
    for (size_t i = 0; i < eventCount; ++i)
    {
        ipath path(eventPathsC[i]);
        weak<FileSystem::WatchPoint> point = FileSystem::WatchPoint::getWatchPoint(path);
        if (point)
        {
            be_info("refreshing path %s" | path);
            point->signalDirty();
        }
    }
}


ref<Folder::Watch> WatchPoint::addWatch(weak<DiskFolder> folder, const BugEngine::ipath& path)
{
    static FileSystemWatchProcessQueue s_queue;


    ipath absolutePath("");
    if (path[0] != istring(""))
    {
        char prefixBuffer[PATH_MAX+1];
        char* prefix = getcwd(prefixBuffer, PATH_MAX);
        absolutePath += ipath(prefix);
    }
    absolutePath += path;

    weak<WatchPoint> point = WatchPoint::getWatchPointOrCreate(absolutePath);
    ref<DiskFolder::Watch> result = ref<DiskFolder::Watch>::create(Arena::filesystem(), folder, point);
    return result;
}


}}
