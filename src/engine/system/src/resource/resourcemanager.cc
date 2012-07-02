/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/resource/resourcemanager.hh>

namespace BugEngine
{

ResourceManager::ResourceManager()
: m_loaders(Arena::resource())
, m_tickets(Arena::resource())
{
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::attach(raw<const RTTI::Class> classinfo, weak<IResourceLoader> loader)
{
    for (minitl::vector<LoaderInfo>::iterator it = m_loaders.begin(); it != m_loaders.end(); ++it)
    {
        be_assert_recover(it->loader != loader, "registering twice the same loader for class %s" | classinfo->name, return);
    }
    be_info("registering loader for type %s" | classinfo->name);
    LoaderInfo loaderInfo;
    loaderInfo.classinfo = classinfo;
    loaderInfo.loader = loader;
    m_loaders.push_back(loaderInfo);
}

void ResourceManager::detach(raw<const RTTI::Class> classinfo, weak<const IResourceLoader> loader)
{
    for (minitl::vector<LoaderInfo>::iterator it = m_loaders.begin(); it != m_loaders.end();)
    {
        if (it->classinfo == classinfo && it->loader == loader)
        {
            be_info("unregistering loader for type %s" | classinfo->name);
            it = m_loaders.erase(it);
            return;
        }
        else
        {
            ++it;
        }
    }
    be_error("loader was not in the list of loaders for type %s" | classinfo->name);
}

void ResourceManager::load(raw<const RTTI::Class> classinfo, weak<const Resource> resource) const
{
    int loadedCount = 0;
    for (minitl::vector<LoaderInfo>::const_iterator it = m_loaders.begin(); it != m_loaders.end(); ++it)
    {
        if (classinfo->isA(it->classinfo))
        {
            resource->load(it->loader);
            loadedCount++;
        }
    }
    be_assert(loadedCount, "resource of type %s has no loader registered" | classinfo->name);
}

void ResourceManager::unload(raw<const RTTI::Class> classinfo, weak<const Resource> resource) const
{
    for (minitl::vector<LoaderInfo>::const_iterator it = m_loaders.begin(); it != m_loaders.end(); ++it)
    {
        if (classinfo->isA(it->classinfo))
        {
            resource->unload(it->loader);
        }
    }
}

void ResourceManager::addTicket(weak<IResourceLoader> loader, weak<const Resource> resource, weak<const File> file)
{
    Ticket ticket;
    ticket.loader = loader;
    ticket.resource = resource;
    ticket.ticket = file->beginRead(0, 0, Arena::temporary());
    ticket.progress = 0;
    m_tickets.push_back(ticket);
}

size_t ResourceManager::updateTickets()
{
    for (minitl::vector< Ticket >::iterator it = m_tickets.begin(); it != m_tickets.end(); /*nothing*/)
    {
        if (it->ticket->processed != it->progress)
        {
            if (it->ticket->done())
            {
                it->loader->onTicketLoaded(it->resource, it->ticket->buffer);
                it = m_tickets.erase(it);
            }
            else
            {
                it->progress = it->ticket->processed;
                it->loader->onTicketUpdated(it->resource, it->ticket->buffer, it->progress);
                ++it;
            }
        }
        else
        {
            ++it;
        }
    }
    return m_tickets.size();
}

}
