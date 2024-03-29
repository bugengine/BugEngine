/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/filesystem/stdafx.h>
#include <bugengine/filesystem/zipfolder.script.hh>
#include <zipfile.hh>

#include <unzip.h>

namespace BugEngine {

ZipFolder::ZipFolder(void* handle, ipath path, Folder::ScanPolicy scanPolicy)
    : m_handle(handle)
    , m_path(path)
{
    be_info("opening zip folder %s" | path);
    if(scanPolicy != Folder::ScanNone)
    {
        refresh(scanPolicy);
    }
}

ZipFolder::ZipFolder(const ipath& zippath, Folder::ScanPolicy scanPolicy) : m_handle(0), m_path("")
{
    m_handle = unzOpen(zippath.str().name);
    if(!m_handle)
    {
        be_error("Could not open zip %s/" | zippath);
    }

    if(scanPolicy != Folder::ScanNone)
    {
        refresh(scanPolicy);
    }
}

ZipFolder::~ZipFolder()
{
    ScopedCriticalSection lock(m_lock);
    if(m_handle)
    {
        unzClose(m_handle);
        m_handle = 0;
    }
}

void ZipFolder::doRefresh(Folder::ScanPolicy scanPolicy)
{
    Folder::doRefresh(scanPolicy);
    Folder::ScanPolicy newPolicy
        = (scanPolicy == Folder::ScanRecursive) ? Folder::ScanRecursive : Folder::ScanNone;
    if(m_handle)
    {
        ScopedCriticalSection lock(m_lock);
        if(unzGoToFirstFile(m_handle) == UNZ_OK)
        {
            minitl::vector< istring > subdirs(Arena::stack());
            do
            {
                unz_file_info info;
                char          filepath[4096];
                unzGetCurrentFileInfo(m_handle, &info, filepath, sizeof(filepath), 0, 0, 0, 0);
                ipath   path(filepath);
                istring filename = path.pop_back();
                if(path == m_path)
                {
                    unz_file_pos filePos;
                    unzGetFilePos(m_handle, &filePos);
                    ifilename fullFilePath = path + ifilename(filename);
                    m_files.push_back(minitl::make_tuple(
                        filename, ref< ZipFile >::create(Arena::filesystem(), m_handle,
                                                         fullFilePath, info, filePos)));
                }
                else if(path.size() >= 1)
                {
                    istring directory = path.pop_back();
                    if(m_path == path)
                    {
                        subdirs.push_back(directory);
                    }
                }
            } while(unzGoToNextFile(m_handle) == UNZ_OK);

            for(minitl::vector< istring >::const_iterator it = subdirs.begin(); it != subdirs.end();
                ++it)
            {
                if(openFolderNoLock(ipath(*it)) == weak< Folder >())
                {
                    ipath path = m_path;
                    path.push_back(*it);
                    m_folders.push_back(minitl::make_tuple(
                        *it,
                        ref< ZipFolder >::create(Arena::filesystem(), m_handle, path, newPolicy)));
                }
            }
        }
    }
}

void ZipFolder::onChanged()
{
}

}  // namespace BugEngine
