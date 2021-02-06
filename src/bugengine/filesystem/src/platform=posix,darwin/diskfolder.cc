/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/filesystem/stdafx.h>
#include <bugengine/filesystem/diskfolder.script.hh>
#include <sys/stat.h>
#include <sys/types.h>
#include <watchpoint.hh>
#include DIRENT_H
#include <errno.h>
#include <limits.h>
#include <posix/file.hh>
#include <stdio.h>

namespace BugEngine {

static void createDirectory(const ipath& path, Folder::CreatePolicy policy)
{
    be_assert(policy != Folder::CreateNone, "invalid policy given to createDirectory");
    if(policy == Folder::CreateRecursive)
    {
        ipath parent = path;
        parent.pop_back();
        createDirectory(parent, policy);
    }
    ipath::Filename p = path.str();
    if(mkdir(p.name, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
    {
        if(errno != EEXIST)
        {
            perror("");
        }
    }
}

DiskFolder::DiskFolder(const ipath& diskpath, Folder::ScanPolicy scanPolicy,
                       Folder::CreatePolicy createPolicy)
    : m_path(diskpath)
    , m_index(0)
    , m_watch()
{
    be_forceuse(m_index);
    if(createPolicy != Folder::CreateNone)
    {
        createDirectory(diskpath, createPolicy);
    }
    ipath::Filename pathname = m_path.str();
    m_handle.ptrHandle       = opendir(pathname.name);
    if(!m_handle.ptrHandle)
    {
        be_error("Could not open directory %s: %s" | diskpath | strerror(errno));
    }
    else
    {
        m_watch = FileSystem::WatchPoint::addWatch(this, diskpath);
    }

    if(scanPolicy != Folder::ScanNone)
    {
        refresh(scanPolicy);
    }
}

DiskFolder::~DiskFolder()
{
    ScopedCriticalSection lock(m_lock);
    if(m_handle.ptrHandle)
    {
        closedir((DIR*)m_handle.ptrHandle);
        m_handle.ptrHandle = 0;
    }
}

void DiskFolder::doRefresh(Folder::ScanPolicy scanPolicy)
{
    Folder::doRefresh(scanPolicy);
    Folder::ScanPolicy newPolicy
        = (scanPolicy == Folder::ScanRecursive) ? Folder::ScanRecursive : Folder::ScanNone;
    if(m_handle.ptrHandle)
    {
        ScopedCriticalSection lock(m_lock);
        rewinddir((DIR*)m_handle.ptrHandle);
        while(dirent* d = readdir((DIR*)m_handle.ptrHandle))
        {
            if(strcmp(d->d_name, ".") == 0) continue;
            if(strcmp(d->d_name, "..") == 0) continue;
            istring name = d->d_name;
            ipath   p    = m_path;
            p.push_back(name);
            ipath::Filename filename = p.str();
            struct stat     s;
            stat(filename.name, &s);
            if(errno == 0)
            {
                be_error("could not stat file %s: %s(%d)" | filename.name | strerror(errno)
                         | errno);
            }
            else if(s.st_mode & S_IFDIR)
            {
                ref< DiskFolder > newFolder = ref< DiskFolder >::create(
                    Arena::filesystem(), p, newPolicy, Folder::CreateNone);
                m_folders.push_back(minitl::make_tuple(name, newFolder));
            }
            else
            {
                File::Media media(File::Media::Disk, s.st_dev, s.st_ino);
                ref< File > newFile
                    = ref< PosixFile >::create(Arena::filesystem(), ipath(m_path) + ifilename(name),
                                               media, s.st_size, s.st_mtime);
                m_files.push_back(minitl::make_tuple(name, newFile));
            }
        }
    }
}

weak< File > DiskFolder::createFile(const istring& name)
{
    struct stat s;
    errno = 0;
    char  fullPathBuffer[PATH_MAX];
    char* fullPath = realpath(m_path.str().name, fullPathBuffer);
    if(!fullPath)
    {
        be_error("could not get real path of %s: %s(%d)" | m_path | strerror(errno) | errno);
        return ref< File >();
    }
    strcat(fullPath, "/");
    strcat(fullPath, name.c_str());
    FILE* f = fopen(fullPath, "w");
    if(f == 0)
    {
        be_error("could not create file %s: %s(%d)" | fullPath | strerror(errno) | errno);
        return ref< File >();
    }
    fclose(f);
    if(stat(fullPath, &s) != 0)
    {
        be_error("could not create file %s: %s(%d)" | fullPath | strerror(errno) | errno);
        return ref< File >();
    }

    ScopedCriticalSection lock(m_lock);
    ref< File >           result = ref< PosixFile >::create(
        Arena::filesystem(), m_path + ifilename(name),
        File::Media(File::Media::Disk, s.st_dev, s.st_ino), s.st_size, s.st_mtime);

    for(minitl::vector< minitl::tuple< istring, ref< File > > >::iterator it = m_files.begin();
        it != m_files.end(); ++it)
    {
        if(it->first == name)
        {
            it->second = result;
            return result;
        }
    }
    m_files.push_back(minitl::make_tuple(name, result));
    return result;
}

void DiskFolder::onChanged()
{
    if(m_handle.ptrHandle)
    {
        ScopedCriticalSection lock(m_lock);
        rewinddir((DIR*)m_handle.ptrHandle);
        while(dirent* d = readdir((DIR*)m_handle.ptrHandle))
        {
            if(strcmp(d->d_name, ".") == 0) continue;
            if(strcmp(d->d_name, "..") == 0) continue;
            istring name = d->d_name;
            ipath   p    = m_path;
            p.push_back(name);
            ipath::Filename filename = p.str();
            struct stat     s;
            int             result = stat(filename.name, &s);
            if(result != 0)
            {
                be_error("could not stat file %s: %s(%d)" | filename.name | strerror(errno)
                         | errno);
            }
            else if(s.st_mode & S_IFDIR)
            {
                /* TODO: deleted folder */
                bool exists = false;

                for(minitl::vector< minitl::tuple< istring, ref< Folder > > >::iterator it
                    = m_folders.begin();
                    it != m_folders.end(); ++it)
                {
                    if(it->first == name)
                    {
                        exists = true;
                        break;
                    }
                }
                if(!exists)
                {
                    be_info("new folder: %s" | p);
                    ref< DiskFolder > newFolder = ref< DiskFolder >::create(
                        Arena::filesystem(), p, Folder::ScanNone, Folder::CreateNone);
                    m_folders.push_back(minitl::make_tuple(name, newFolder));
                }
            }
            else
            {
                /* TODO: deleted file */
                bool exists = false;

                for(minitl::vector< minitl::tuple< istring, ref< File > > >::iterator it
                    = m_files.begin();
                    it != m_files.end(); ++it)
                {
                    if(it->first == name)
                    {
                        be_checked_cast< PosixFile >(it->second)->refresh(s.st_size, s.st_mtime);
                        exists = true;
                    }
                }
                if(!exists)
                {
                    be_info("new file: %s" | p);
                    File::Media media(File::Media::Disk, s.st_dev, s.st_ino);
                    ref< File > newFile = ref< PosixFile >::create(Arena::filesystem(),
                                                                   ipath(m_path) + ifilename(name),
                                                                   media, s.st_size, s.st_mtime);
                    m_files.push_back(minitl::make_tuple(name, newFile));
                }
            }
        }
    }
}

}  // namespace BugEngine
