/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_FILESYSTEM_WATCHPOINT_HH_
#define BE_FILESYSTEM_WATCHPOINT_HH_
/*****************************************************************************/
#include    <filesystem/diskfolder.script.hh>

namespace BugEngine { namespace FileSystem
{

class WatchPoint : public minitl::refcountable
{
private:
    typedef minitl::vector< weak<DiskFolder::Watch> >                   WatchVector;
    typedef minitl::vector< minitl::pair<istring, ref<WatchPoint> > >   ChildrenVector;
    WatchVector     m_watches;
    ChildrenVector  m_children;
public:
    WatchPoint();
    ~WatchPoint();
public:
    static weak<WatchPoint> getWatchPointOrCreate(const ipath& path);
    static weak<WatchPoint> getWatchPoint(const ipath& path);

    void addWatch(weak<DiskFolder::Watch> watch);
    void removeWatch(weak<DiskFolder::Watch> watch);

    void signalDirty();
private:
    static ref<WatchPoint> s_root;
};

}}

/*****************************************************************************/
#endif
