/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_FILE_FOLDER_SCRIPT_HH_
#define BE_SYSTEM_FILE_FOLDER_SCRIPT_HH_
/*****************************************************************************/
#include    <core/string/istring.hh>
#include    <system/file/file.script.hh>

namespace BugEngine
{

class be_api(SYSTEM) Folder : public minitl::refcountable
{
    friend class File;
protected:
    minitl::vector< minitl::pair<istring, ref<File> > >     m_files;
    minitl::vector< minitl::pair<istring, ref<Folder> > >   m_folders;
    minitl::vector< minitl::pair<istring, ref<Folder> > >   m_mounts;
    bool                                                    m_upToDate;
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
published:
    weak<File>      openFile(istring name);
    weak<File>      openFile(ifilename name);
    weak<Folder>    openFolder(istring name);
    weak<Folder>    openFolder(ipath name);
    void            mount(istring name, ref<Folder> folder);
    void            mount(ipath name, ref<Folder> folder);
    void            umount(istring name);
    void            umount(ipath name);

    void            refresh(ScanPolicy scanPolicy);
private:
    static void     pushTicket(ref<const File::Ticket> ticket);
};

}

/*****************************************************************************/
#endif
