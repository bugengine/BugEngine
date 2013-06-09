/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_FILESYSTEM_FILE_DISKFOLDER_SCRIPT_HH_
#define BE_FILESYSTEM_FILE_DISKFOLDER_SCRIPT_HH_
/*****************************************************************************/
#include    <filesystem/folder.script.hh>

namespace BugEngine
{

class FileSystemWatch;

class be_api(FILESYSTEM) DiskFolder : public Folder
{
public:
    class Watch : public minitl::refcountable
    {
        friend class FileSystemWatch;
    private:
        u8  m_state;
    public:
        Watch();
        ~Watch();

        bool isDirty() const;
        bool isDirtyRecursive() const;
    };
private:
    union Handle
    {
        void*   ptrHandle;
        u64     intHandle;
    };
    ipath       m_path;
    Handle      m_handle;
    u32         m_index;
    ref<Watch>  m_watch;
private:
    void doRefresh(Folder::ScanPolicy scanPolicy) override;
published:
    DiskFolder(const ipath& diskpath, Folder::ScanPolicy scanPolicy = Folder::ScanRecursive, Folder::CreatePolicy createPolicy = Folder::CreateOne);
    ~DiskFolder();

    weak<File> createFile(const istring& name);
};

}

/*****************************************************************************/
#endif
