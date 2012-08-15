/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <resource/stdafx.h>
#include    <resource/resourcemanager.hh>

namespace BugEngine
{

ResourceManager::LoaderInfo::LoaderInfo()
    :   classinfo()
    ,   loaders(Arena::resource())
    ,   resources()
{
}

ResourceManager::ResourceManager()
    :   m_loaders(Arena::resource(), 1024)
    ,   m_tickets(Arena::resource())
{
}

ResourceManager::~ResourceManager()
{
}

ResourceManager::LoaderInfo& ResourceManager::getLoaderInfo(raw<const RTTI::Class> classinfo)
{
    for (minitl::array<LoaderInfo>::iterator it = m_loaders.begin(); it != m_loaders.end(); ++it)
    {
        if (classinfo->isA(it->classinfo))
            return *it;
        if (!it->classinfo)
        {
            it->classinfo = classinfo;
            return *it;
        }
    }
    be_notreached();
    return m_loaders[0];
}

void ResourceManager::attach(raw<const RTTI::Class> classinfo, weak<IResourceLoader> loader)
{
    LoaderInfo& info = getLoaderInfo(classinfo);
    for (minitl::vector< weak<IResourceLoader> >::iterator it = info.loaders.begin(); it != info.loaders.end(); ++it)
    {
        be_assert_recover(*it != loader, "registering twice the same loader for class %s" | classinfo->name, return);
    }
    be_info("registering loader for type %s" | classinfo->name);
    info.loaders.push_back(loader);
    for (minitl::intrusive_list<const Resource, 2>::iterator resource = info.resources.begin(); resource != info.resources.end(); ++resource)
    {
        resource->load(loader);
    }
}

void ResourceManager::detach(raw<const RTTI::Class> classinfo, weak<const IResourceLoader> loader)
{
    LoaderInfo& info = getLoaderInfo(classinfo);
    for (minitl::vector< weak<IResourceLoader> >::iterator it = info.loaders.begin(); it != info.loaders.end();)
    {
        if (*it == loader)
        {
            be_info("unregistering loader for type %s" | classinfo->name);
            for (minitl::intrusive_list<const Resource, 2>::iterator resource = info.resources.begin(); resource != info.resources.end(); ++resource)
            {
                resource->unload(*it);
            }
            it = info.loaders.erase(it);
            return;
        }
        else
        {
            ++it;
        }
    }
    be_error("loader was not in the list of loaders for type %s" | classinfo->name);
}

void ResourceManager::load(raw<const RTTI::Class> classinfo, weak<const Resource> resource)
{
    LoaderInfo& info = getLoaderInfo(classinfo);
    for (minitl::vector< weak<IResourceLoader> >::const_iterator it = info.loaders.begin(); it != info.loaders.end(); ++it)
    {
        resource->load(*it);
    }
    info.resources.push_back(*resource.operator->());
}

void ResourceManager::unload(raw<const RTTI::Class> classinfo, weak<const Resource> resource)
{
    LoaderInfo& info = getLoaderInfo(classinfo);
    for (minitl::vector< weak<IResourceLoader> >::const_iterator it = info.loaders.begin(); it != info.loaders.end(); ++it)
    {
        resource->unload(*it);
    }
    resource->unhook();
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
        if (it->ticket->error)
        {
            be_error("resource loading error");
            it = m_tickets.erase(it);
        }
        else if (it->ticket->done())
        {
            it->loader->onTicketLoaded(it->resource, it->ticket->buffer);
            it = m_tickets.erase(it);
        }
        else if (it->ticket->processed != it->progress)
        {
            it->progress = it->ticket->processed;
            it->loader->onTicketUpdated(it->resource, it->ticket->buffer, it->progress);
            ++it;
        }
        else
        {
            ++it;
        }
    }
    return m_tickets.size();
}

}
