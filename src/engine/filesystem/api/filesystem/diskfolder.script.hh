/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_FILESYSTEM_FILE_DISKFOLDER_SCRIPT_HH_
#define BE_FILESYSTEM_FILE_DISKFOLDER_SCRIPT_HH_
/**************************************************************************************************/
#include    <filesystem/folder.script.hh>

namespace BugEngine
{

class FileSystemWatch;

class be_api(FILESYSTEM) DiskFolder : public Folder
{
public:
    class Watch;
    friend class Watch;
private:
    union Handle
    {
        void*   ptrHandle;
        u64     intHandle;
    };
    ipath               m_path;
    Handle              m_handle;
    u32                 m_index;
    ref<Folder::Watch>  m_watch;
private:
    void doRefresh(Folder::ScanPolicy scanPolicy) override;
    void onChanged() override;
published:
    DiskFolder(const ipath& diskpath, Folder::ScanPolicy scanPolicy = Folder::ScanRecursive, Folder::CreatePolicy createPolicy = Folder::CreateOne);
    ~DiskFolder();

    weak<File> createFile(const istring& name);
};

}

/**************************************************************************************************/
#endif
