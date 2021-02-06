/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/filesystem/stdafx.h>
#include <bugengine/core/threads/criticalsection.hh>
#include <watchpoint.hh>

namespace BugEngine { namespace FileSystem {

static CriticalSection s_lock;

WatchPoint::WatchPoint(weak< WatchPoint > parent)
    : m_watches(Arena::filesystem())
    , m_children(Arena::filesystem())
    , m_parent(parent)
    , m_recursiveWatchCount(0)
{
}

WatchPoint::WatchPoint()
    : m_watches(Arena::filesystem())
    , m_children(Arena::filesystem())
    , m_parent()
    , m_recursiveWatchCount(0)
{
}

WatchPoint::~WatchPoint()
{
}

ref< WatchPoint > WatchPoint::getWatchPointOrCreate(const ipath& path)
{
    ref< WatchPoint > result(s_root);
    for(u32 i = 0; i < path.size(); ++i)
    {
        ref< WatchPoint >     nextChild;
        ScopedCriticalSection lock(s_lock);
        for(ChildrenVector::iterator it = result->m_children.begin();
            it != result->m_children.end(); ++it)
        {
            if(it->first == path[i])
            {
                nextChild = it->second;
                break;
            }
        }
        if(!nextChild)
        {
            ref< WatchPoint > newChild = ref< WatchPoint >::create(Arena::filesystem(), result);
            result->m_children.push_back(minitl::make_tuple(path[i], newChild));
            nextChild = newChild;
        }
        result = nextChild;
    }
    return result;
}

ref< WatchPoint > WatchPoint::getWatchPoint(const ipath& path)
{
    ref< WatchPoint > result(s_root);
    for(u32 i = 0; i < path.size(); ++i)
    {
        ref< WatchPoint >     nextChild;
        ScopedCriticalSection lock(s_lock);
        for(ChildrenVector::iterator it = result->m_children.begin();
            it != result->m_children.end(); ++it)
        {
            if(it->first == path[i])
            {
                nextChild = it->second;
                break;
            }
        }
        if(!nextChild)
        {
            return nextChild;
        }
        else
        {
            result = nextChild;
        }
    }
    return result;
}

void WatchPoint::addWatch(weak< Folder::Watch > watch)
{
    ScopedCriticalSection lock(s_lock);
    m_watches.push_back(watch);

    for(weak< WatchPoint > p = this; p; p = p->m_parent)
    {
        p->m_recursiveWatchCount++;
    }
}

void WatchPoint::removeWatch(weak< Folder::Watch > watch)
{
    ScopedCriticalSection lock(s_lock);
    for(WatchVector::iterator it = m_watches.begin(); it != m_watches.end(); ++it)
    {
        if(*it == watch)
        {
            m_watches.erase(it);

            for(weak< WatchPoint > p = this; p; p = p->m_parent)
            {
                p->m_recursiveWatchCount--;
            }

            return;
        }
    }
    be_notreached();
}

void WatchPoint::signalDirty()
{
    WatchVector v(Arena::temporary());
    {
        ScopedCriticalSection lock(s_lock);
        v = m_watches;
    }
    for(WatchVector::iterator it = v.begin(); it != v.end(); ++it)
    {
        (*it)->signal();
    }
}

void WatchPoint::cleanupTree()
{
    ScopedCriticalSection lock(s_lock);
    for(ChildrenVector::iterator it = m_children.begin(); it != m_children.end();)
    {
        if(it->second->m_recursiveWatchCount == 0)
        {
            it = m_children.erase(it);
        }
        else
        {
            it->second->cleanupTree();
            ++it;
        }
    }
}

void WatchPoint::cleanup()
{
    s_root->cleanupTree();
}

ref< WatchPoint > WatchPoint::s_root = ref< WatchPoint >::create(Arena::general());

}}  // namespace BugEngine::FileSystem
