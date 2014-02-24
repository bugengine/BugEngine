/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_FILESYSTEM_DISKWATCH_HH_
#define BE_FILESYSTEM_DISKWATCH_HH_
/**************************************************************************************************/
#include    <filesystem/diskfolder.script.hh>

namespace BugEngine
{

namespace FileSystem
{
class WatchPoint;
}

class DiskFolder::Watch : public Folder::Watch
{
private:
    weak<FileSystem::WatchPoint> m_watchPoint;
public:
    Watch(weak<DiskFolder> folder, weak<FileSystem::WatchPoint> watchPoint);
    ~Watch();
};

}

/**************************************************************************************************/
#endif
