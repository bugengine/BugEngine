/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_FILESYSTEM_ZIPFOLDER_SCRIPT_HH_
#define BE_FILESYSTEM_ZIPFOLDER_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/filesystem/stdafx.h>
#include <bugengine/filesystem/folder.script.hh>

namespace BugEngine {

class FileSystemWatch;

class be_api(FILESYSTEM) ZipFolder : public Folder
{
private:
    void* m_handle;
    ipath m_path;

private:
    void doRefresh(Folder::ScanPolicy scanPolicy) override;
    void onChanged() override;

public:
    ZipFolder(void* handle, ipath path, Folder::ScanPolicy scanPolicy = Folder::ScanRecursive);
published:
    ZipFolder(const ipath& zippath, Folder::ScanPolicy scanPolicy = Folder::ScanRecursive);
    ~ZipFolder();
};

}  // namespace BugEngine

/**************************************************************************************************/
#endif
