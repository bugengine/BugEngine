/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <main/stdafx.h>
#include    <main/scripting.hh>


namespace BugEngine
{

IScriptEngine::IScriptEngine(Allocator& arena)
    :   m_tickets(arena)
{
}

IScriptEngine::~IScriptEngine()
{
}

void IScriptEngine::update()
{
    for (minitl::vector< ref<const File::Ticket> >::iterator it = m_tickets.begin(); it != m_tickets.end(); /*nothing*/)
    {
        if ((*it)->done())
        {
            runBuffer((*it)->buffer);
            it = m_tickets.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void IScriptEngine::loadFile(weak<const File> file, Allocator& arena)
{
    m_tickets.push_back(file->beginRead(0, 0, arena));
}

}
