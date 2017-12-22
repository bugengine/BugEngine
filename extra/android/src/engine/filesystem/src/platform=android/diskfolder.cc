/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <filesystem/stdafx.h>
#include    <filesystem/diskfolder.script.hh>
#include    <filesystem/zipfolder.script.hh>
#include    <watchpoint.hh>
#include    <sys/types.h>
#include    <sys/stat.h>
#include    <dirent.h>
#include    <errno.h>
#include    <stdio.h>
#include    <posix/file.hh>
#include    <zipfile.hh>
#include    <unzip.h>

extern BE_IMPORT const char* s_packagePath;

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
    ,   m_index(m_path[0] == "apk:" ? 1 : 0)
    ,   m_watch()
{
    if (m_index == 0)
    {
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
                    m_folders.push_back(minitl::make_tuple(name, newFolder));
                }
                else
                {
                    File::Media media(File::Media::Disk, s.st_dev, s.st_ino);
                    ref<File> newFile = ref<PosixFile>::create(Arena::filesystem(), ipath(m_path) + ifilename(name), media, s.st_size, s.st_mtime);
                    m_files.push_back(minitl::make_tuple(name, newFile));
                }
            }
        }
        else
        {
            ipath relativePath = m_path;
            relativePath.pop_front();
            if (unzGoToFirstFile(m_handle.ptrHandle) == UNZ_OK)
            {
                minitl::vector<istring> subdirs(Arena::stack());
                do
                {
                    unz_file_info info;
                    char filepath[4096];
                    unzGetCurrentFileInfo(m_handle.ptrHandle, &info, filepath, sizeof(filepath), 0, 0, 0, 0);
                    ipath path(filepath);
                    istring filename = path.pop_back();
                    if (path == relativePath)
                    {
                        unz_file_pos filePos;
                        unzGetFilePos(m_handle.ptrHandle, &filePos);
                        ifilename filepath = path + ifilename(filename);
                        m_files.push_back(minitl::make_tuple(filename, ref<ZipFile>::create(Arena::filesystem(), m_handle.ptrHandle, filepath, info, filePos)));
                    }
                    else if (path.size() >= 1)
                    {
                        istring directory = path.pop_back();
                        if (relativePath == path)
                        {
                            subdirs.push_back(directory);
                        }
                    }
                } while (unzGoToNextFile(m_handle.ptrHandle) == UNZ_OK);

                for (minitl::vector<istring>::const_iterator it = subdirs.begin(); it != subdirs.end(); ++it)
                {
                    be_info("%s" | *it);
                    if (openFolderNoLock(ipath(*it)) == weak<Folder>())
                    {
                        be_info("> %s" | *it);
                        ipath path = relativePath;
                        path.push_back(*it);
                        m_folders.push_back(minitl::make_tuple(*it, ref<ZipFolder>::create(Arena::filesystem(), m_handle.ptrHandle, path, newPolicy)));
                    }
                    be_info("%s" | *it);
                }
                be_info("test");
            }
        }
    }
}

weak<File> DiskFolder::createFile(const istring& name)
{
    if (m_index == 0)
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

        for (minitl::vector< minitl::tuple<istring, ref<File> > >::iterator it = m_files.begin(); it != m_files.end(); ++it)
        {
            if (it->first == name)
            {
                it->second = result;
                return result;
            }
        }
        m_files.push_back(minitl::make_tuple(name, result));
        return result;
    }
    else
    {
        be_error("can't create new file: read-only APK filesystem");
        return weak<File>();
    }
}

void DiskFolder::onChanged()
{
}

}
