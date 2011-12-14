/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_BUGENGINE_SCRIPTENGINE_INL_
#define BE_BUGENGINE_SCRIPTENGINE_INL_
/*****************************************************************************/
#include    <bugengine/stdafx.h>
#include    <bugengine/scriptengine.hh>
#include    <system/resource/resourcemanager.hh>

namespace BugEngine
{

template< typename T >
ScriptEngine<T>::ScriptEngine(Allocator& arena, weak<ResourceManager> manager)
    :   IResourceLoader()
    ,   m_scriptArena(arena)
    ,   m_manager(manager)
    ,   m_tickets(arena)
{
    m_manager->attach<T>(this);
}

template< typename T >
ScriptEngine<T>::~ScriptEngine()
{
    m_manager->detach<T>(this);
}
#ifdef NOT_IMPLEMENTED
template< typename T >
void ScriptEngine<T>::update()
{
    for (minitl::vector< minitl::pair< ref<const File::Ticket>, weak<const Script> > >::iterator it = m_tickets.begin(); it != m_tickets.end(); /*nothing*/)
    {
        if (it->first->done())
        {
            runBuffer(be_checked_cast<const T>(it->second), it->first->buffer);
            it = m_tickets.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
#endif

template< typename T >
ResourceHandle ScriptEngine<T>::load(weak<const Resource> resource)
{
    m_tickets.push_back(minitl::make_pair(be_checked_cast<const T>(resource)->m_file->beginRead(0, 0, tempArena()), be_checked_cast<const T>(resource)));
    return ResourceHandle();
}

template< typename T >
void ScriptEngine<T>::unload(const ResourceHandle& handle)
{
}

}

/*****************************************************************************/
#endif
