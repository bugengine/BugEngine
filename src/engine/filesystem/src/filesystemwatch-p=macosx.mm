/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <filesystem/stdafx.h>
#include    <filesystemwatch.hh>
#include    <core/threads/thread.hh>
#include    <core/environment.hh>
#include    <kernel/interlocked.hh>

#include    <Foundation/Foundation.h>
#include    <CoreFoundation/CoreFoundation.h>
#include    <CoreServices/CoreServices.h>

namespace BugEngine
{

class FileSystemWatch::FileSystemWatchProcessQueue : public minitl::pointer
{
private:
    CFStringRef m_Path;
    Thread m_thread;
    iptr<CFRunLoopRef> m_runLoop;
private:
    static intptr_t runFileSystemWatch(intptr_t p1, intptr_t p2);
    static void onFileSsystemEvent(ConstFSEventStreamRef streamRef,
                                   void *clientCallBackInfo,
                                   size_t numEvents,
                                   void *eventPaths,
                                   const FSEventStreamEventFlags eventFlags[],
                                   const FSEventStreamEventId eventIds[]);
public:
    FileSystemWatchProcessQueue();
    ~FileSystemWatchProcessQueue();

    ref<DiskFolder::Watch> addFolder(const ipath& path);
};

static CFStringRef getApplicationDirectory()
{
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    NSString* bundlePath = [[NSBundle mainBundle] bundlePath];
    CFStringRef result = CFStringCreateWithCString(kCFAllocatorDefault, [bundlePath UTF8String], kCFStringEncodingUTF8);
    [pool release];
    return result;
}

FileSystemWatch::FileSystemWatchProcessQueue::FileSystemWatchProcessQueue()
    :   m_Path(getApplicationDirectory())
    ,   m_thread("FileSystemWatch", &FileSystemWatchProcessQueue::runFileSystemWatch, (intptr_t)this, 0)
    ,   m_runLoop(0)
{
}

FileSystemWatch::FileSystemWatchProcessQueue::~FileSystemWatchProcessQueue()
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

intptr_t FileSystemWatch::FileSystemWatchProcessQueue::runFileSystemWatch(intptr_t p1, intptr_t /*p2*/)
{
    FileSystemWatchProcessQueue* watch = reinterpret_cast<FileSystemWatchProcessQueue*>(p1);
    CFRunLoopRef runLoop = CFRunLoopGetCurrent();
    CFRetain(runLoop);
    watch->m_runLoop = &runLoop;

    printf("watching path %s", Environment::getEnvironment().getDataDirectory().str().name);

    FSEventStreamContext context = { 0, (void*)watch, NULL, NULL, NULL };
    FSEventStreamRef eventStream = FSEventStreamCreate(kCFAllocatorDefault, &FileSystemWatchProcessQueue::onFileSsystemEvent, &context, CFArrayCreate(kCFAllocatorDefault, (const void**)&watch->m_Path, 1, 0), kFSEventStreamEventIdSinceNow, 3.0, kFSEventStreamCreateFlagNone);
    FSEventStreamScheduleWithRunLoop(eventStream, runLoop, kCFRunLoopCommonModes);
    FSEventStreamStart(eventStream);

    CFRunLoopRun();

    FSEventStreamStop(eventStream);
    FSEventStreamInvalidate(eventStream);
    FSEventStreamRelease(eventStream);

    return 0;
}

void FileSystemWatch::FileSystemWatchProcessQueue::onFileSsystemEvent(ConstFSEventStreamRef /*streamRef*/,
                                                                      void* /*clientCallBackInfo*/,
                                                                      size_t eventCount,
                                                                      void* eventPaths,
                                                                      const FSEventStreamEventFlags /*eventFlags*/[],
                                                                      const FSEventStreamEventId /*eventIds*/[])
{
    const char** eventPathsC = (const char**)eventPaths;
    for (size_t i = 0; i < eventCount; ++i)
    {
        be_info("Got filesystem event on object %s" | eventPathsC[i]);
    }
}

ref<DiskFolder::Watch> FileSystemWatch::FileSystemWatchProcessQueue::addFolder(const BugEngine::ipath &path)
{
    be_forceuse(path);
    return ref<DiskFolder::Watch>::create(Arena::filesystem());
}

FileSystemWatch::FileSystemWatch()
    :   m_queue(scoped<FileSystemWatchProcessQueue>::create(BugEngine::Arena::filesystem()))
{
}

FileSystemWatch::~FileSystemWatch()
{
}

ref<DiskFolder::Watch> FileSystemWatch::watchDirectory(const BugEngine::ipath &folder)
{
    return s_fileSystemWatch.m_queue->addFolder(folder);
}

FileSystemWatch FileSystemWatch::s_fileSystemWatch;

}
