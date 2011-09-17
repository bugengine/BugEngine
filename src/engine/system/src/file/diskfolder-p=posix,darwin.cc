/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/file/diskfolder.script.hh>
#include    <sys/types.h>
#include    <sys/stat.h>
#include    DIRENT_H
#include    <errno.h>


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
    minitl::format<1024u> p = path.str();
    if (mkdir(p.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
    {
        if (errno != EEXIST)
        {
            perror("");
        }
    }
}

DiskFolder::DiskFolder(const ipath& diskpath, Folder::ScanPolicy scanPolicy, Folder::CreatePolicy createPolicy)
    :   m_path(diskpath)
{
    if(createPolicy != Folder::CreateNone) { createDirectory(diskpath, createPolicy); }
    minitl::format<1024u> pathname = m_path.str();
    m_handle.ptrHandle = opendir(pathname.c_str());
    if (!m_handle.ptrHandle)
    {
        perror("");
    }

    if (scanPolicy != Folder::ScanNone)
    {
        refresh(scanPolicy);
    }
}

DiskFolder::~DiskFolder()
{
    closedir((DIR*)m_handle.ptrHandle);
}

void DiskFolder::doRefresh(Folder::ScanPolicy scanPolicy)
{
    if (m_handle.ptrHandle)
    {
        minitl::format<1024u> pathname = m_path.str();
        rewinddir((DIR*)m_handle.ptrHandle);
        while(dirent* d = readdir((DIR*)m_handle.ptrHandle))
        {
            if (d->d_name[0] == '.' && d->d_name[1] == 0)
                continue;
            if (d->d_name[0] == '.' && d->d_name[1] == '.' && d->d_name[2] == 0)
                continue;
            istring name = d->d_name;
            minitl::format<1024u> filename = pathname;
            filename.append("/");
            filename.append(d->d_name);
            struct stat s;
            stat(filename.c_str(), &s);
            if (s.st_mode & S_IFDIR)
            {
                be_debug("found dir: %s" | name);
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
                m_folders.push_back(minitl::make_pair(name, ref<DiskFolder>::create(fsArena(), m_path+ipath(name), scanPolicy == Folder::ScanRecursive?Folder::ScanRecursive : Folder::ScanNone, Folder::CreateNone)));
            }
            else
            {
                /*todo*/
                be_info("found file: %s" | name);
            }
        }
    }
}

}
