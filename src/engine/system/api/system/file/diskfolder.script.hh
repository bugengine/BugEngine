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
    ipath   m_path;
    Handle  m_handle;
private:
    void doRefresh(Folder::ScanPolicy scanPolicy) override;
published:
    DiskFolder(const ipath& diskpath, Folder::ScanPolicy scanPolicy = Folder::ScanRecursive, Folder::CreatePolicy createPolicy = Folder::CreateOne);
    ~DiskFolder();

};

}

/*****************************************************************************/
#endif
