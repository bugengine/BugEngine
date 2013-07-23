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
    friend class DiskFolder::Watch;
private:
    typedef minitl::vector< weak<Folder::Watch> >                       WatchVector;
    typedef minitl::vector< minitl::pair<istring, ref<WatchPoint> > >   ChildrenVector;
    WatchVector                 m_watches;
    ChildrenVector              m_children;
    weak<WatchPoint>            m_parent;
    u32                         m_recursiveWatchCount;
public:
    WatchPoint();
    WatchPoint(weak<WatchPoint> parent);
    ~WatchPoint();
public:
    void signalDirty();

    static ref<Folder::Watch> addWatch(weak<DiskFolder> folder, const ipath& path);
    static ref<WatchPoint> getWatchPoint(const ipath& path);
private:
    void addWatch(weak<Folder::Watch> watch);
    void removeWatch(weak<Folder::Watch> watch);
    void cleanupTree();

    static ref<WatchPoint> getWatchPointOrCreate(const ipath& path);
    static void cleanup();
    static ref<WatchPoint> s_root;
};

}}

/*****************************************************************************/
#endif
