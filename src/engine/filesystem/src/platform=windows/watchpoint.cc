/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <filesystem/stdafx.h>
#include    <watchpoint.hh>

namespace BugEngine { namespace FileSystem
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
    friend class WatchThread;
public:
    enum RequestType
    {
        Add,
        Remove
    };
private:
    RequestType     m_type;
    ipath           m_path;
    ref<WatchPoint> m_watchpoint;
public:
    WatchRequest(RequestType type, const ipath& path, ref<WatchPoint> watchpoint)
        :   m_type(type)
        ,   m_path(path)
        ,   m_watchpoint(watchpoint)
    {
    }
};

class WatchThread : public minitl::refcountable
{
private:
    static const u32 s_maximumWatchCount = MAXIMUM_WAIT_OBJECTS-1;
    HANDLE                                                                  m_thread;
    HANDLE                                                                  m_semaphore;
    minitl::vector< minitl::pair<HANDLE, weak<FileSystem::WatchPoint> > >   m_watches;
    minitl::istack<WatchRequest>                                            m_requests;
    u32                                                                     m_watchCount;
private:
    static unsigned long WINAPI doWatchFolders(void* params);
public:
    WatchThread();
    ~WatchThread();

    bool add(const ipath& path, ref<FileSystem::WatchPoint> watchpoint);
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
        HANDLE handles[s_maximumWatchCount+1];
        int i = 0;
        handles[i++] = watchThread->m_semaphore;
        for (minitl::vector<  minitl::pair<HANDLE, weak<FileSystem::WatchPoint> > >::const_iterator it = watchThread->m_watches.begin(); it != watchThread->m_watches.end(); ++it)
        {
            handles[i++] = it->first;
        }
        DWORD result = WaitForMultipleObjects(1 + (DWORD)watchThread->m_watches.size(), handles, FALSE, INFINITE);
        if (result == WAIT_OBJECT_0)
        {
            WatchRequest* request = watchThread->m_requests.pop();
            if (!request)
            {
                break;
            }
            else
            {
                DWORD flags = FILE_NOTIFY_CHANGE_FILE_NAME
                    | FILE_NOTIFY_CHANGE_DIR_NAME
                    | FILE_NOTIFY_CHANGE_ATTRIBUTES
                    | FILE_NOTIFY_CHANGE_SIZE
                    | FILE_NOTIFY_CHANGE_LAST_WRITE;
                HANDLE handle = FindFirstChangeNotificationA(request->m_path.str().name, FALSE, flags);
                watchThread->m_watches.push_back(minitl::make_pair(handle, request->m_watchpoint));
                request->~WatchRequest();
                //Arena::temporary().free(request);
            }
        }
        else if (result >= WAIT_OBJECT_0 + 1 && result <= WAIT_OBJECT_0 + watchThread->m_watches.size())
        {
            int index = result - WAIT_OBJECT_0 - 1;
            watchThread->m_watches[index].second->signalDirty();
            FindNextChangeNotification(watchThread->m_watches[index].first);
        }
        else if (result >= WAIT_ABANDONED_0 + 1 && result <= WAIT_ABANDONED_0 + watchThread->m_watches.size())
        {
            be_notreached();
            int index = result - WAIT_ABANDONED_0 - 1;
            FindNextChangeNotification(watchThread->m_watches[index].first);
        }
        else
        {
            char *errorMessage = 0;
            int errorCode = ::GetLastError();
            FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                NULL,
                errorCode,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                reinterpret_cast<LPSTR>(&errorMessage),
                0,
                NULL);
            be_error("file watch wait interrupted: %s" | errorMessage);
            ::LocalFree(errorMessage);
            be_notreached();
        }
    }
    return 0;
}

WatchThread::WatchThread()
    :   m_thread(CreateThread(0, 0, &WatchThread::doWatchFolders, this, 0, 0))
    ,   m_semaphore(CreateSemaphore(NULL, 0, 65535, NULL))
    ,   m_watches(Arena::filesystem())
    ,   m_watchCount(0)
{
}

WatchThread::~WatchThread()
{
    ReleaseSemaphore(m_semaphore, 1, NULL);
    DWORD result = WaitForSingleObject(m_thread, 2000);
    be_assert(result != WAIT_TIMEOUT, "timed out when waiting for filesystem watch thread");
    CloseHandle(m_thread);
    CloseHandle(m_semaphore);
    for (minitl::vector< minitl::pair<HANDLE, weak<FileSystem::WatchPoint> > >::iterator it = m_watches.begin(); it != m_watches.end(); ++it)
    {
        FindCloseChangeNotification(it->first);
    }
}

bool WatchThread::add(const ipath& path, ref<FileSystem::WatchPoint> watchpoint)
{
    if (m_watchCount < s_maximumWatchCount)
    {
        WatchRequest* request = new (Arena::temporary()) WatchRequest(WatchRequest::Add, path, watchpoint);
        m_requests.push(request);
        m_watchCount++;
        ReleaseSemaphore(m_semaphore, 1, 0);
        return true;
    }
    else
    {
        return false;
    }
}


ref<Folder::Watch> WatchPoint::addWatch(weak<DiskFolder> folder, const BugEngine::ipath& path)
{
    static minitl::vector< ref<WatchThread> >   s_threads(Arena::filesystem());

    ref<WatchPoint> watchpoint = getWatchPoint(path);
    if (!watchpoint)
    {
        watchpoint = getWatchPointOrCreate(path);
        bool found = false;
        for (minitl::vector< ref<WatchThread> >::iterator it = s_threads.begin(); it != s_threads.end(); ++it)
        {
            if ((*it)->add(path, watchpoint))
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            ref<WatchThread> thread = ref<WatchThread>::create(Arena::filesystem());
            thread->add(path, watchpoint);
        }
    }
    ref<DiskFolder::Watch> result = ref<DiskFolder::Watch>::create(Arena::filesystem(), folder, watchpoint);
    watchpoint->addWatch(result);
    return result;
}

}}
