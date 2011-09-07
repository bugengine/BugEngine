/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_FILE_DISKFOLDER_SCRIPT_HH_
#define BE_SYSTEM_FILE_DISKFOLDER_SCRIPT_HH_
/*****************************************************************************/
#include    <system/file/folder.script.hh>

namespace BugEngine
{

class be_api(SYSTEM) DiskFolder : public Folder
{
private:
    union Handle
    {
        void*   ptrHandle;
        u64     intHandle;
    };
    Handle  m_handle;
published:
    enum DiskScanPolicy
    {
        ScanNone,
        ScanRoot,
        ScanRecursive
    };
    enum DiskCreatePolicy
    {
        CreateNone,
        CreateOne,
        CreateRecursive
    };
    DiskFolder(const ipath& diskpath, DiskScanPolicy scanPolicy = ScanRecursive, DiskCreatePolicy createPolicy = CreateOne);
    ~DiskFolder();
};

}

/*****************************************************************************/
#endif
