/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/filesystem/stdafx.h>
#include <bugengine/filesystem/diskfolder.script.hh>
#include <watchpoint.hh>
#include <windows/file.hh>

namespace BugEngine {

static u64 getTimeStamp(FILETIME time)
{
    return u64(time.dwLowDateTime) + (u64(time.dwHighDateTime) << 32);
}

static void createDirectory(const ipath& path, Folder::CreatePolicy policy)
{
    be_assert(policy != Folder::CreateNone, "invalid policy given to createDirectory");
    if(policy == Folder::CreateRecursive)
    {
        ipath parent = path;
        parent.pop_back();
        createDirectory(parent, policy);
    }
    ipath::Filename pathname = path.str('\\');
    if(!CreateDirectoryA(pathname.name, 0))
    {
        int err = GetLastError();
        if(err == ERROR_ALREADY_EXISTS)
        {
            return;
        }
        else
        {
            be_info("directory %s could not be created: error code %d" | pathname.name | err);
        }
    }
}

static i_u32 s_diskIndex = i_u32::create(0);

DiskFolder::DiskFolder(const ipath& diskpath, Folder::ScanPolicy scanPolicy,
                       Folder::CreatePolicy createPolicy)
    : m_path(diskpath)
    , m_index(s_diskIndex++)
    , m_watch()
{
    if(createPolicy != Folder::CreateNone)
    {
        createDirectory(diskpath, createPolicy);
    }
    ipath::Filename pathname = m_path.str('\\');
    m_handle.ptrHandle       = CreateFileA(pathname.name, GENERIC_READ,
                                     FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, 0,
                                     OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, 0);
    if(m_handle.ptrHandle == INVALID_HANDLE_VALUE)
    {
        int errorCode = ::GetLastError();
        be_info("Directory %s could not be opened: (%d)" | pathname.name | errorCode);
    }
    else
    {
        m_watch = FileSystem::WatchPoint::addWatch(this, diskpath);
    }

    if(scanPolicy != Folder::ScanNone)
    {
        refresh(scanPolicy);
    }
}

DiskFolder::~DiskFolder()
{
    CloseHandle(m_handle.ptrHandle);
}

void DiskFolder::doRefresh(Folder::ScanPolicy scanPolicy)
{
    Folder::doRefresh(scanPolicy);
    if(m_handle.ptrHandle)
    {
        WIN32_FIND_DATA     data;
        ifilename::Filename pathname = (m_path + ifilename("*")).str('\\');
        HANDLE              h        = FindFirstFile(pathname.name, &data);
        if(h != INVALID_HANDLE_VALUE)
        {
            do
            {
                if(data.cFileName[0] == '.' && data.cFileName[1] == 0) continue;
                if(data.cFileName[0] == '.' && data.cFileName[1] == '.' && data.cFileName[2] == 0)
                    continue;
                istring name = data.cFileName;
                if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    for(minitl::vector< minitl::tuple< istring, ref< Folder > > >::iterator it
                        = m_folders.begin();
                        it != m_folders.end(); ++it)
                    {
                        if(it->first == name)
                        {
                            continue;
                        }
                    }
                    m_folders.push_back(minitl::make_tuple(
                        name, ref< DiskFolder >::create(Arena::filesystem(), m_path + ipath(name),
                                                        scanPolicy, Folder::CreateNone)));
                }
                else
                {
                    u64 size = data.nFileSizeHigh;
                    size <<= 32;
                    size += data.nFileSizeLow;
                    ref< Win32File > newFile
                        = ref< Win32File >::create(Arena::filesystem(), m_path + ifilename(name),
                                                   File::Media(File::Media::Disk, m_index, 0), size,
                                                   getTimeStamp(data.ftLastWriteTime));
                    m_files.push_back(minitl::make_tuple(name, newFile));
                }
            } while(FindNextFile(h, &data));
            FindClose(h);
        }
    }
}

weak< File > DiskFolder::createFile(const istring& name)
{
    const ifilename::Filename path = (m_path + ifilename(name)).str('\\');
    HANDLE                    h = CreateFileA(path.name, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
    if(h == INVALID_HANDLE_VALUE)
    {
        int errorCode = ::GetLastError();
        be_info("file %s (%s) could not be opened: CreateFile returned an error (%d)" | m_path
                | path.name | errorCode);
        return weak< File >();
    }
    else
    {
        CloseHandle(h);
        WIN32_FIND_DATA data;
        h = FindFirstFile(path.name, &data);
        if(h == INVALID_HANDLE_VALUE)
        {
            int errorCode = ::GetLastError();
            be_info("file %s (%s) could not be opened: CreateFile returned an error (%d)" | m_path
                    | path.name | errorCode);
            return weak< File >();
        }
        FindClose(h);
        ref< File > result = ref< Win32File >::create(Arena::filesystem(), m_path + ifilename(name),
                                                      File::Media(File::Media::Disk, m_index, 0), 0,
                                                      getTimeStamp(data.ftLastWriteTime));
        for(minitl::vector< minitl::tuple< istring, ref< File > > >::iterator it = m_files.begin();
            it != m_files.end(); ++it)
        {
            if(it->first == name)
            {
                it->second = result;
                return result;
            }
        }
        m_files.push_back(minitl::make_tuple(name, result));
        return result;
    }
}

void DiskFolder::onChanged()
{
    if(m_handle.ptrHandle)
    {
        WIN32_FIND_DATA     data;
        ifilename::Filename pathname = (m_path + ifilename("*")).str('\\');
        HANDLE              h        = FindFirstFile(pathname.name, &data);
        if(h != INVALID_HANDLE_VALUE)
        {
            do
            {
                if(data.cFileName[0] == '.' && data.cFileName[1] == 0) continue;
                if(data.cFileName[0] == '.' && data.cFileName[1] == '.' && data.cFileName[2] == 0)
                    continue;
                istring name = data.cFileName;
                if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    bool exists = false;
                    for(minitl::vector< minitl::tuple< istring, ref< Folder > > >::iterator it
                        = m_folders.begin();
                        it != m_folders.end(); ++it)
                    {
                        if(it->first == name)
                        {
                            exists = true;
                            break;
                        }
                    }
                    if(!exists)
                    {
                        m_folders.push_back(minitl::make_tuple(
                            name,
                            ref< DiskFolder >::create(Arena::filesystem(), m_path + ipath(name),
                                                      Folder::ScanNone, Folder::CreateNone)));
                    }
                }
                else
                {
                    bool exists = false;
                    for(minitl::vector< minitl::tuple< istring, ref< File > > >::iterator it
                        = m_files.begin();
                        it != m_files.end(); ++it)
                    {
                        if(it->first == name)
                        {
                            exists   = true;
                            u64 size = data.nFileSizeHigh;
                            size <<= 32;
                            size += data.nFileSizeLow;
                            be_checked_cast< Win32File >(it->second)
                                ->refresh(size, getTimeStamp(data.ftLastWriteTime));
                            break;
                        }
                    }
                    if(!exists)
                    {
                        u64 size = data.nFileSizeHigh;
                        size <<= 32;
                        size += data.nFileSizeLow;
                        ref< Win32File > newFile = ref< Win32File >::create(
                            Arena::filesystem(), m_path + ifilename(name),
                            File::Media(File::Media::Disk, m_index, 0), size,
                            getTimeStamp(data.ftLastWriteTime));
                        m_files.push_back(minitl::make_tuple(name, newFile));
                    }
                }
            } while(FindNextFile(h, &data));
            FindClose(h);
        }
    }
}

}  // namespace BugEngine
