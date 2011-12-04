/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MAIN_SCRIPTENGINE_INL_
#define BE_MAIN_SCRIPTENGINE_INL_
/*****************************************************************************/
#include    <main/stdafx.h>
#include    <main/scriptengine.hh>


namespace BugEngine
{

template< typename T >
ScriptEngine<T>::ScriptEngine(Allocator& arena)
    :   IScriptEngine()
    ,   m_scriptArena(arena)
    ,   m_tickets(arena)
{
    ResourceLoaders::attach<T, ScriptEngine<T> >(this, &ScriptEngine::loadScript, &ScriptEngine::unloadScript);
}

template< typename T >
ScriptEngine<T>::~ScriptEngine()
{
    ResourceLoaders::detach<T, ScriptEngine<T> >(this);
}

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

template< typename T >
ResourceHandle ScriptEngine<T>::loadScript(weak<const T> resource)
{
    m_tickets.push_back(minitl::make_pair(resource->m_file->beginRead(0, 0, tempArena()), resource));
    return ResourceHandle();
}

template< typename T >
void ScriptEngine<T>::unloadScript(const ResourceHandle& handle)
{
}

}

/*****************************************************************************/
#endif
