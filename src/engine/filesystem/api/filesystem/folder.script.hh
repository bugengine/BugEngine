/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_FILESYSTEM_FILE_FOLDER_SCRIPT_HH_
#define BE_FILESYSTEM_FILE_FOLDER_SCRIPT_HH_
/**************************************************************************************************/
#include    <filesystem/stdafx.h>
#include    <core/string/istring.hh>
#include    <filesystem/file.script.hh>
#include    <core/threads/criticalsection.hh>

namespace BugEngine
{

class be_api(FILESYSTEM) Folder : public minitl::refcountable
{
protected:
    CriticalSection                                         m_lock;
    minitl::vector< minitl::tuple<istring, ref<File> > >    m_files;
    minitl::vector< minitl::tuple<istring, ref<Folder> > >  m_folders;
    minitl::vector< minitl::tuple<istring, ref<Folder> > >  m_mounts;
    bool                                                    m_upToDate;
public:
    class Watch : public minitl::refcountable
    {
    private:
        weak<Folder>  m_folder;
    public:
        Watch(weak<Folder> folder);
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
    virtual void    doRefresh(ScanPolicy scanPolicy) = 0;
    virtual void    onChanged() = 0;
    void            refresh(ScanPolicy scanPolicy);
published:
    weak<File>      openFile(istring name);
    weak<File>      openFile(ifilename name);
    weak<Folder>    openFolder(istring name);
    weak<Folder>    openFolder(ipath name);
    void            mount(istring name, ref<Folder> folder);
    void            mount(ipath name, ref<Folder> folder);
    void            umount(istring name);
    void            umount(ipath name);
};

}

/**************************************************************************************************/
#endif
