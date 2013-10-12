/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <filesystem/stdafx.h>
#include    <filesystem/diskfolder.script.hh>
#include    <watchpoint.hh>
#include    <sys/types.h>
#include    <sys/stat.h>
#include    <dirent.h>
#include    <errno.h>
#include    <stdio.h>
#include    <posix/file.hh>
#include    <android/assetfile.hh>
#include    <unzip.h>

extern BE_IMPORT const char* s_packagePath;

namespace BugEngine
{

static void createDirectory(const ipath& path, Folder::CreatePolicy policy)
{
    be_assert (policy != Folder::CreateNone, "invalid policy given to createDirectory");
    if (policy == Folder::CreateRecursive)
    {
        BugEngine::ipath parent = path;
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
    ,   m_index(m_path[0] == "apk:" ? 1 : 0)
    ,   m_watch()
{
    be_forceuse(m_index);
    if(createPolicy != Folder::CreateNone)
    {
        createDirectory(diskpath, createPolicy);
    }
    if (m_index == 0)
    {
        ipath::Filename pathname = m_path.str();
        m_handle.ptrHandle = opendir(pathname.name);
        if (!m_handle.ptrHandle)
        {
            be_error("Could not open directory %s: %s" | diskpath | strerror(errno));
        }
    }
    else
    {
        ipath package_path = m_path;
        package_path.pop_front();
        zlib_filefunc_def_s defs;
        fill_fopen_filefunc(&defs);
        m_handle.ptrHandle = unzOpen2(s_packagePath, &defs);
        if (!m_handle.ptrHandle)
        {
            be_error("Could not open directory %s/" | m_path);
        }
    }

    if (scanPolicy != Folder::ScanNone)
    {
        refresh(scanPolicy);
    }
}

DiskFolder::~DiskFolder()
{
    ScopedCriticalSection lock(m_lock);
    if (m_index == 0)
    {
        if (m_handle.ptrHandle)
        {
            closedir((DIR*)m_handle.ptrHandle);
            m_handle.ptrHandle = 0;
        }
    }
    else
    {
        if (m_handle.ptrHandle)
        {
            unzClose(m_handle.ptrHandle);
            m_handle.ptrHandle = 0;
        }
    }
}

void DiskFolder::doRefresh(Folder::ScanPolicy scanPolicy)
{
    Folder::doRefresh(scanPolicy);
    Folder::ScanPolicy newPolicy = (scanPolicy == Folder::ScanRecursive) ? Folder::ScanRecursive : Folder::ScanNone;
    if (m_handle.ptrHandle)
    {
        ScopedCriticalSection lock(m_lock);
        if (m_index == 0)
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
                    ref<DiskFolder> newFolder = ref<DiskFolder>::create(Arena::filesystem(), p, newPolicy, Folder::CreateNone);
                    m_folders.push_back(minitl::make_pair(name, newFolder));
                }
                else
                {
                    File::Media media(File::Media::Disk, s.st_dev, s.st_ino);
                    ref<File> newFile = ref<PosixFile>::create(Arena::filesystem(), ipath(m_path) + ifilename(name), media, s.st_size, s.st_mtime);
                    m_files.push_back(minitl::make_pair(name, newFile));
                }
            }
        }
        else
        {
            if (unzGoToFirstFile(m_handle.ptrHandle) == UNZ_OK)
            {
                do
                {
                    unz_file_info info;
                    char filename[4096];
                    unzGetCurrentFileInfo(m_handle.ptrHandle, &info, filename, sizeof(filename), 0, 0, 0, 0);
                    be_info(filename);
                } while (unzGoToNextFile(m_handle.ptrHandle) == UNZ_OK);
            }
        }
    }
}

weak<File> DiskFolder::createFile(const istring& name)
{
    be_assert_recover(m_path[0] != istring("apk:"), "can't create a file in the Package directory", return weak<File>());
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

    ScopedCriticalSection lock(m_lock);
    ref<File> result = ref<PosixFile>::create(
                Arena::filesystem(),
                m_path+ifilename(name),
                File::Media(File::Media::Disk, s.st_dev, s.st_ino),
                s.st_size,
                s.st_mtime);

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

void DiskFolder::onChanged()
{
}

}
