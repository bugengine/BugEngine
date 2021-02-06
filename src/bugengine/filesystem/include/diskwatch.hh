/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_FILESYSTEM_DISKWATCH_HH_
#define BE_FILESYSTEM_DISKWATCH_HH_
/**************************************************************************************************/
#include <bugengine/filesystem/stdafx.h>
#include <bugengine/filesystem/diskfolder.script.hh>

namespace BugEngine {

namespace FileSystem {
class WatchPoint;
}

class DiskFolder::Watch : public Folder::Watch
{
private:
    weak< FileSystem::WatchPoint > m_watchPoint;

public:
    Watch(weak< DiskFolder > folder, weak< FileSystem::WatchPoint > watchPoint);
    ~Watch();
};

}  // namespace BugEngine

/**************************************************************************************************/
#endif
