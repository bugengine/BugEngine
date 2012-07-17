/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/file/diskfolder.script.hh>
#include    <win32/file.hh>


namespace BugEngine
{

static void createDirectory(const ipath& path, Folder::CreatePolicy policy)
{
    be_assert (policy != Folder::CreateNone, "invalid policy given to createDirectory");
    if (policy == Folder::CreateRecursive)
    {
        ipath parent = path;
        parent.pop_back();
        createDirectory(parent, policy);
    }
    minitl::format<1024u> pathname = path.str();
    if (!CreateDirectoryA(pathname.c_str(), 0))
    {
        int err = GetLastError();
        if (err == ERROR_ALREADY_EXISTS)
        {
            return;
        }
        else
        {
            char *errorMessage = 0;
            FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                NULL,
                err,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                reinterpret_cast<LPSTR>(&errorMessage),
                0,
                NULL);
            be_info("Directory %s could not be created: error code %d (%s)" | path | err | errorMessage);
        }
    }

}

static i_u32 s_diskIndex = 0;

DiskFolder::DiskFolder(const ipath& diskpath, Folder::ScanPolicy scanPolicy, Folder::CreatePolicy createPolicy)
    :   m_path(diskpath)
    ,   m_index(s_diskIndex++)
{
    if(createPolicy != Folder::CreateNone) { createDirectory(diskpath, createPolicy); }
    minitl::format<1024u> pathname = m_path.str();
    m_handle.ptrHandle = CreateFileA (pathname.c_str(),
                                      GENERIC_READ,
                                      FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,
                                      0,
                                      OPEN_EXISTING,
                                      FILE_FLAG_BACKUP_SEMANTICS,
                                      0);
    if(m_handle.ptrHandle == INVALID_HANDLE_VALUE)
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
        be_info("Directory %s could not be opened: (%d) %s" | diskpath | errorCode | errorMessage);
        ::LocalFree(errorMessage);
    }

    if (scanPolicy != Folder::ScanNone)
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
    if (m_handle.ptrHandle)
    {
        WIN32_FIND_DATA data;
        minitl::format<1024u> pathname = m_path.str();
        pathname.append("\\*");
        HANDLE h = FindFirstFile(pathname.c_str(), &data);
        if (h != INVALID_HANDLE_VALUE)
        {
            do
            {
                if (data.cFileName[0] == '.' && data.cFileName[1] == 0)
                    continue;
                if (data.cFileName[0] == '.' && data.cFileName[1] == '.' && data.cFileName[2] == 0)
                    continue;
                istring name = data.cFileName;
                if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    for (minitl::vector< minitl::pair<istring, ref<Folder> > >::iterator it = m_folders.begin(); it != m_folders.end(); ++it)
                    {
                        if (it->first == name)
                        {
                            continue;
                        }
                    }
                    m_folders.push_back(minitl::make_pair(name, ref<DiskFolder>::create(Arena::filesystem(), m_path+ipath(name), scanPolicy, Folder::CreateNone)));
                }
                else
                {
                    u64 size = data.nFileSizeHigh;
                    size <<= 32;
                    size += data.nFileSizeLow;
                    m_files.push_back(minitl::make_pair(name, ref<Win32File>::create(Arena::filesystem(), m_path+ifilename(name), File::Media(File::Media::Disk, m_index, 0), size)));
                }
            } while (FindNextFile(h, &data));
            FindClose(h);
        }
    }
}

weak<File> DiskFolder::createFile(const istring& name)
{
    const minitl::format<1024u> path = (m_path+ifilename(name)).str();
    HANDLE h = CreateFileA ( path.c_str(),
                             GENERIC_WRITE,
                             0,
                             0,
                             CREATE_ALWAYS,
                             0,
                             0);
    if (h == INVALID_HANDLE_VALUE)
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
        be_error("File %s could not be created: CreateFile returned an error (%d) %s" | path | errorCode | errorMessage);
        ::LocalFree(errorMessage);
        return weak<File>();
    }
    else
    {
        CloseHandle(h);
        WIN32_FIND_DATA data;
        HANDLE h = FindFirstFile(path.c_str(), &data);
        if (h == INVALID_HANDLE_VALUE)
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
            be_error("File %s could not be created: FindFirstFile returned an error (%d) %s" | path | errorCode | errorMessage);
            ::LocalFree(errorMessage);
            return weak<File>();
        }
        FindClose(h);
        ref<File> result = ref<Win32File>::create(
                    Arena::filesystem(),
                    m_path+ifilename(name),
                    File::Media(File::Media::Disk, m_index, 0),
                    0);
        for (minitl::vector< minitl::pair<istring, ref<File> > >::iterator it = m_files.begin(); it != m_files.end(); ++it)
        {
            if (it->first == name)
            {
                it->second = result;
                return result;
            }
        }
        m_files.push_back(minitl::make_pair(name, result));
        return result;
    }
}


}
