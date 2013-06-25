/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <filesystem/stdafx.h>
#include    <filesystemwatch.hh>
#include    <watch.hh>
#include    <watchpoint.hh>
#include    <minitl/vector.hh>
#include    <kernel/interlocked_stack.hh>

namespace BugEngine
{

#define MS_VC_EXCEPTION 0x406D1388
#pragma pack(push,8)
struct THREADNAME_INFO
{
    DWORD dwType;
    LPCSTR szName;
    DWORD dwThreadID;
    DWORD dwFlags;
};
#pragma pack(pop)

class WatchRequest : public minitl::istack<WatchRequest>::node
{
    enum RequestType
    {
        Add,
        Remove
    };
    ipath   path;
};

class WatchThread : public minitl::refcountable
{
private:
    HANDLE                                                                  m_thread;
    HANDLE                                                                  m_semaphore;
    minitl::vector< minitl::pair<HANDLE, weak<FileSystem::WatchPoint> > >   m_watches;
    minitl::istack<WatchRequest>                                            m_requests;
private:
    static unsigned long WINAPI doWatchFolders(void* params);
public:
    WatchThread();
    ~WatchThread();
};

static void setThreadName(const istring& name)
{
#ifdef _MSC_VER
    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = name.c_str();
    info.dwThreadID = GetCurrentThreadId();
    info.dwFlags = 0;
    __try
    {
        RaiseException(MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(ULONG_PTR), (ULONG_PTR*)&info);
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
    }
#else
    be_forceuse(name);
#endif
}

unsigned long WINAPI WatchThread::doWatchFolders(void* params)
{
    WatchThread* watchThread = reinterpret_cast<WatchThread*>(params);
    static i_u32 s_threadIndex = i_u32::Zero;
    setThreadName(istring(minitl::format<1024u>("FileSystem watch %d") | s_threadIndex++));

    while (true)
    {
        HANDLE handles[MAXIMUM_WAIT_OBJECTS];
        handles[0] = watchThread->m_semaphore;
        DWORD result = WaitForMultipleObjects(1 + (DWORD)watchThread->m_watches.size(), handles, FALSE, INFINITE);
        if (result == WAIT_OBJECT_0)
        {
            WatchRequest* request = watchThread->m_requests.pop();
            if (!request)
            {
                break;
            }
        }
        else if (result >= WAIT_OBJECT_0 + 1 && result <= WAIT_OBJECT_0 + watchThread->m_watches.size())
        {
            int index = result - WAIT_OBJECT_0 - 1;
            be_forceuse(index);
        }
        else
        {
        }
    }
    return 0;
}

WatchThread::WatchThread()
    :   m_thread(CreateThread(0, 0, &WatchThread::doWatchFolders, this, 0, 0))
    ,   m_semaphore(CreateSemaphore(NULL, 0, 65535, NULL))
    ,   m_watches(Arena::filesystem())
{
}

WatchThread::~WatchThread()
{
    ReleaseSemaphore(m_semaphore, 1, NULL);
    DWORD result = WaitForSingleObject(m_thread, 2000);
    be_assert(result != WAIT_TIMEOUT, "timed out when waiting for filesystem watch thread");
    CloseHandle(m_thread);
    CloseHandle(m_semaphore);
}

class FileSystemWatch::FileSystemWatchProcessQueue : public minitl::pointer
{
private:
    CriticalSection                     m_lock;
    minitl::vector< ref<WatchThread> >  m_threads;
public:
    FileSystemWatchProcessQueue();
    ~FileSystemWatchProcessQueue();

    ref<DiskFolder::Watch> watchDirectory(weak<DiskFolder> folder, const BugEngine::ipath& path);
};

FileSystemWatch::FileSystemWatchProcessQueue::FileSystemWatchProcessQueue()
    :   m_threads(Arena::filesystem())
{
}

FileSystemWatch::FileSystemWatchProcessQueue::~FileSystemWatchProcessQueue()
{
}

ref<DiskFolder::Watch> FileSystemWatch::FileSystemWatchProcessQueue::watchDirectory(weak<DiskFolder> folder, const BugEngine::ipath& path)
{
    ScopedCriticalSection lock(m_lock);
    weak<FileSystem::WatchPoint> watchpoint = FileSystem::WatchPoint::getWatchPoint(path);
    if (!watchpoint)
    {
       watchpoint = FileSystem::WatchPoint::getWatchPointOrCreate(path);
       bool added = false;
       for (minitl::vector< ref<WatchThread> >::iterator it = m_threads.begin(); it != m_threads.end(); ++it)
       {
           if (false)
           {
               added = true;
           }
       }
       if (!added)
       {
           ref<WatchThread> newThread = ref<WatchThread>::create(Arena::filesystem());
           //newThread->addWatch(watchpoint);
           m_threads.push_back(newThread);
       }
    }
    return ref<FileSystem::DiskWatch>::create(Arena::filesystem(), folder, watchpoint);
}

FileSystemWatch::FileSystemWatch()
    :   m_queue(scoped<FileSystemWatchProcessQueue>::create(BugEngine::Arena::filesystem()))
{
}

FileSystemWatch::~FileSystemWatch()
{
}

ref<DiskFolder::Watch> FileSystemWatch::watchDirectory(weak<DiskFolder> folder, const BugEngine::ipath& path)
{
    return s_fileSystemWatch.m_queue->watchDirectory(folder, path);
}

FileSystemWatch FileSystemWatch::s_fileSystemWatch;

}
