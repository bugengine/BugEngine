/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/file/diskfolder.script.hh>


namespace BugEngine
{

static void createDirectory(const ipath& path, DiskFolder::DiskCreatePolicy policy)
{
    be_assert (policy != DiskFolder::CreateNone, "invalid policy given to createDirectory");
    if (policy == DiskFolder::CreateRecursive)
    {
        ipath parent = path;
        parent.pop_back();
        createDirectory(parent, policy);
    }
    if (!CreateDirectoryA(path.str().c_str(), 0))
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

DiskFolder::DiskFolder(const ipath& diskpath, DiskScanPolicy /*scanPolicy*/, DiskCreatePolicy createPolicy)
{
    if(createPolicy != CreateNone) { createDirectory(diskpath, createPolicy); }
    m_handle.ptrHandle = CreateFileA (diskpath.str().c_str(),
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
}

DiskFolder::~DiskFolder()
{
    CloseHandle(m_handle.ptrHandle);
}

}
