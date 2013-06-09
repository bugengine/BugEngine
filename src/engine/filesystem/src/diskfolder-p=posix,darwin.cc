/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <filesystem/stdafx.h>
#include    <filesystem/diskfolder.script.hh>
#include    <filesystemwatch.hh>
#include    <sys/types.h>
#include    <sys/stat.h>
#include    DIRENT_H
#include    <errno.h>
#include    <stdio.h>
#include    <posix/file.hh>

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
    ipath::Filename p = path.str();
    if (mkdir(p.name, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
    {
        if (errno != EEXIST)
        {
            perror("");
        }
    }
}

DiskFolder::DiskFolder(const ipath& diskpath, Folder::ScanPolicy scanPolicy, Folder::CreatePolicy createPolicy)
    :   m_path(diskpath)
    ,   m_index(0)
    ,   m_watch(FileSystemWatch::watchDirectory(diskpath))
{
    be_forceuse(m_index);
    if(createPolicy != Folder::CreateNone)
    {
        createDirectory(diskpath, createPolicy);
    }
    ipath::Filename pathname = m_path.str();
    m_handle.ptrHandle = opendir(pathname.name);
    if (!m_handle.ptrHandle)
    {
        be_error("Could not open directory %s: %s" | diskpath | strerror(errno));
    }

    if (scanPolicy != Folder::ScanNone)
    {
        refresh(scanPolicy);
    }
}

DiskFolder::~DiskFolder()
{
    if (m_handle.ptrHandle)
    {
        closedir((DIR*)m_handle.ptrHandle);
    }
}

void DiskFolder::doRefresh(Folder::ScanPolicy scanPolicy)
{
    Folder::doRefresh(scanPolicy);
    if (m_handle.ptrHandle)
    {
        rewinddir((DIR*)m_handle.ptrHandle);
        while(dirent* d = readdir((DIR*)m_handle.ptrHandle))
        {
            if (d->d_name[0] == '.' && d->d_name[1] == 0)
                continue;
            if (d->d_name[0] == '.' && d->d_name[1] == '.' && d->d_name[2] == 0)
                continue;
            istring name = d->d_name;
            ipath p = m_path;
            p.push_back(name);
            ipath::Filename filename = p.str();
            struct stat s;
            stat(filename.name, &s);
            if (errno == 0)
            {
                be_error("could not stat file %s: %s(%d)" | filename.name | strerror(errno) | errno);
            }
            else if (s.st_mode & S_IFDIR)
            {
                for (minitl::vector< minitl::pair<istring, ref<Folder> > >::iterator it = m_folders.begin(); it != m_folders.end(); ++it)
                {
                    if (it->first == name)
                    {
                        continue;
                    }
                }
                m_folders.push_back(
                        minitl::make_pair(
                                name,
                                ref<DiskFolder>::create(
                                        Arena::filesystem(),
                                        m_path+ipath(name),
                                        scanPolicy == Folder::ScanRecursive?Folder::ScanRecursive : Folder::ScanNone,
                                        Folder::CreateNone
                                    )));
            }
            else
            {
                m_files.push_back(minitl::make_pair(
                    name,
                    ref<PosixFile>::create(
                            Arena::filesystem(),
                            m_path+ifilename(name),
                            File::Media(File::Media::Disk, s.st_dev, s.st_ino),
                            s.st_size)));
            }
        }
    }
}

weak<File> DiskFolder::createFile(const istring& name)
{
    ifilename::Filename path = (m_path+ifilename(name)).str();
    struct stat s;
    errno = 0;
    FILE* f = fopen(path.name, "w");
    if (f == 0)
    {
        be_error("could not create file %s: %s(%d)" | path.name | strerror(errno) | errno);
        return ref<File>();
    }
    fclose(f);
    if (stat(path.name, &s) != 0)
    {
        be_error("could not create file %s: %s(%d)" | path.name | strerror(errno) | errno);
        return ref<File>();
    }

    ref<File> result = ref<PosixFile>::create(
                Arena::filesystem(),
                m_path+ifilename(name),
                File::Media(File::Media::Disk, s.st_dev, s.st_ino),
                s.st_size);
    for (minitl::vector< minitl::pair<istring, ref<File> > >::iterator it = m_files.begin(); it != m_files.end(); ++it)
    {
        if (it->first == name)
        {
            it->second = result;
            return result;
        }
    }
    m_files.push_back(minitl::make_pair(name, result));
    return result;
}

}
