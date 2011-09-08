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
protected:
    minitl::vector< minitl::pair<istring, ref<File> > >     m_files;
    minitl::vector< minitl::pair<istring, ref<Folder> > >   m_folders;
    minitl::vector< minitl::pair<istring, ref<Folder> > >   m_mounts;
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
published:
    virtual weak<File>          openFile(istring name);
    virtual weak<File>          openFile(ifilename name);
    virtual weak<Folder>        openFolder(istring name);
    virtual weak<Folder>        openFolder(ipath name);
            void                mount(istring name, ref<Folder> folder);
            void                mount(ipath name, ref<Folder> folder);
            void                umount(istring name);
            void                umount(ipath name);

    virtual void refresh(ScanPolicy scanPolicy) = 0;
};

}

/*****************************************************************************/
#endif
