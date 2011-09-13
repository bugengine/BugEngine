/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/file/diskfolder.script.hh>


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

DiskFolder::DiskFolder(const ipath& diskpath, Folder::ScanPolicy scanPolicy, Folder::CreatePolicy createPolicy)
    :   m_path(diskpath)
{
    if(createPolicy != Folder::CreateNone) { createDirectory(diskpath, createPolicy); }
    minitl::format<1024u> pathname = m_path.str();
    m_handle.ptrHandle = CreateFileA (pathname.c_str(),
                                      FILE_ALL_ACCESS,
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
        be_info("Directory %s could not be opened: error code %d (%s)" | diskpath | errorCode | errorMessage);
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
                            if (scanPolicy == Folder::ScanRecursive)
                            {
                                it->second->refresh(scanPolicy);
                                continue;
                            }
                        }
                    }
                    m_folders.push_back(minitl::make_pair(name, ref<DiskFolder>::create(fsArena(), m_path+ipath(name), scanPolicy, Folder::CreateNone)));
                }
                else
                {
                    /*todo*/;
                }
            } while (FindNextFile(h, &data));
            FindClose(h);
        }
    }
}

}
