/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_FILESYSTEM_FILE_FOLDER_SCRIPT_HH_
#define BE_FILESYSTEM_FILE_FOLDER_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/filesystem/stdafx.h>
#include <bugengine/core/string/istring.hh>
#include <bugengine/core/threads/criticalsection.hh>
#include <bugengine/filesystem/file.script.hh>

namespace BugEngine {

class be_api(FILESYSTEM) Folder : public minitl::refcountable
{
protected:
    CriticalSection                                           m_lock;
    minitl::vector< minitl::tuple< istring, ref< File > > >   m_files;
    minitl::vector< minitl::tuple< istring, ref< Folder > > > m_folders;
    minitl::vector< minitl::tuple< istring, ref< Folder > > > m_mounts;
    bool                                                      m_upToDate;

public:
    class Watch : public minitl::refcountable
    {
    private:
        weak< Folder > m_folder;

    public:
        Watch(weak< Folder > folder);
        ~Watch();

        void signal();
    };
    friend class Watch;

protected:
    Folder();
    ~Folder();
published:
    enum CreatePolicy
    {
        CreateNone,
        CreateOne,
        CreateRecursive
    };
    enum ScanPolicy
    {
        ScanNone,
        ScanRoot,
        ScanRecursive
    };

protected:
    virtual void doRefresh(ScanPolicy scanPolicy) = 0;
    virtual void onChanged()                      = 0;
    void         refresh(ScanPolicy scanPolicy);

    weak< File >   openFileNoLock(istring name);
    weak< File >   openFileNoLock(ifilename name);
    weak< Folder > openFolderNoLock(istring name);
    weak< Folder > openFolderNoLock(ipath name);
published:
    weak< File >   openFile(istring name);
    weak< File >   openFile(ifilename name);
    weak< Folder > openFolder(istring name);
    weak< Folder > openFolder(ipath name);
    void           mount(istring name, ref< Folder > folder);
    void           mount(ipath name, ref< Folder > folder);
    void           umount(istring name);
    void           umount(ipath name);
};

}  // namespace BugEngine

/**************************************************************************************************/
#endif
