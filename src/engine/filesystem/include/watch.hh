/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_FILESYSTEM_WATCH_HH_
#define BE_FILESYSTEM_WATCH_HH_
/*****************************************************************************/
#include    <filesystem/diskfolder.script.hh>

namespace BugEngine { namespace FileSystem
{

class WatchPoint;

class DiskWatch : public DiskFolder::Watch
{
private:
    weak<WatchPoint> m_watchPoint;
public:
    DiskWatch(weak<DiskFolder> folder, weak<WatchPoint> watchPoint);
    ~DiskWatch();
};

}}

/*****************************************************************************/
#endif
