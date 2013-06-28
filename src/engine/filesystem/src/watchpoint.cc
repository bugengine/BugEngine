/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <filesystem/stdafx.h>
#include    <watchpoint.hh>
#include    <core/threads/criticalsection.hh>

namespace BugEngine { namespace FileSystem
{

CriticalSection s_lock;
ref<WatchPoint> WatchPoint::s_root = ref<WatchPoint>::create(Arena::filesystem());

WatchPoint::WatchPoint(weak<WatchPoint> parent)
    :   m_watches(Arena::filesystem())
    ,   m_children(Arena::filesystem())
    ,   m_parent(parent)
{
}

WatchPoint::WatchPoint()
    :   m_watches(Arena::filesystem())
    ,   m_children(Arena::filesystem())
    ,   m_parent()
{
}

WatchPoint::~WatchPoint()
{
}

weak<WatchPoint> WatchPoint::getWatchPointOrCreate(const ipath& path)
{
    weak<WatchPoint> result(s_root);
    for (u32 i = 0; i < path.size(); ++i)
    {
        weak<WatchPoint> nextChild;
        ScopedCriticalSection lock(s_lock);
        for (ChildrenVector::iterator it = result->m_children.begin(); it != result->m_children.end(); ++it)
        {
            if (it->first == path[i])
            {
                nextChild = it->second;
                break;
            }
        }
        if (!nextChild)
        {
            ref<WatchPoint> newChild = ref<WatchPoint>::create(Arena::filesystem(), result);
            result->m_children.push_back(minitl::make_pair(path[i], newChild));
            nextChild = newChild;
        }
        result = nextChild;
    }
    return result;
}

weak<WatchPoint> WatchPoint::getWatchPoint(const ipath& path)
{
    weak<WatchPoint> result(s_root);
    for (u32 i = 0; i < path.size(); ++i)
    {
        weak<WatchPoint> nextChild;
        ScopedCriticalSection lock(s_lock);
        for (ChildrenVector::iterator it = result->m_children.begin(); it != result->m_children.end(); ++it)
        {
            if (it->first == path[i])
            {
                nextChild = it->second;
                break;
            }
        }
        if (!nextChild)
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

void WatchPoint::addWatch(weak<DiskFolder::Watch> watch)
{
    ScopedCriticalSection lock(s_lock);
    m_watches.push_back(watch);
}

void WatchPoint::removeWatch(weak<DiskFolder::Watch> watch)
{
    ScopedCriticalSection lock(s_lock);
    for (WatchVector::iterator it = m_watches.begin(); it != m_watches.end(); ++it)
    {
        if (*it == watch)
        {
            m_watches.erase(it);
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
    for (WatchVector::iterator it = v.begin(); it != v.end(); ++it)
    {
        (*it)->signal();
    }
}

}}
