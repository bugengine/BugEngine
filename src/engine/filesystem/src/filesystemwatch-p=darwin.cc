/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <filesystem/stdafx.h>
#include    <filesystemwatch.hh>
#include    <core/threads/thread.hh>
#include    <core/environment.hh>
#include    <CoreFoundation/CoreFoundation.h>
#include    <CoreServices/CoreServices.h>
#include    <kernel/interlocked.hh>

namespace BugEngine { namespace Arena
{

struct FileSystemWatchProcessQueue
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
};

FileSystemWatchProcessQueue::FileSystemWatchProcessQueue()
    :   m_Path(CFStringCreateWithCString(kCFAllocatorDefault, Environment::getEnvironment().getDataDirectory().str().name, kCFStringEncodingUTF8))
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

    printf("watching path %s", Environment::getEnvironment().getDataDirectory().str().name);

    FSEventStreamContext context = { 0, (void*)watch, NULL, NULL, NULL };
    FSEventStreamRef eventStream = FSEventStreamCreate(kCFAllocatorDefault, &FileSystemWatchProcessQueue::onFileSsystemEvent, &context, CFArrayCreate(kCFAllocatorDefault, (const void**)&watch->m_Path, 1, 0), kFSEventStreamEventIdSinceNow, 1.0, kFSEventStreamCreateFlagNone);
    FSEventStreamScheduleWithRunLoop(eventStream, runLoop, kCFRunLoopCommonModes);
    FSEventStreamStart(eventStream);

    CFRunLoopRun();

    FSEventStreamStop(eventStream);
    FSEventStreamInvalidate(eventStream);
    FSEventStreamRelease(eventStream);

    return 0;
}

void FileSystemWatchProcessQueue::onFileSsystemEvent(ConstFSEventStreamRef /*streamRef*/,
                                                     void* /*clientCallBackInfo*/,
                                                     size_t /*eventCount*/,
                                                     void* /*eventPaths*/,
                                                     const FSEventStreamEventFlags /*eventFlags*/[],
                                                     const FSEventStreamEventId /*eventIds*/[])
{
    be_info("Got filesystem event");
}

BE_EXPORT FileSystemWatchProcessQueue s_queue;

}}
