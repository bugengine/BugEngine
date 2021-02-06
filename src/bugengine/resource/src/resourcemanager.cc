/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/resource/stdafx.h>
#include <bugengine/resource/loader.hh>
#include <bugengine/resource/resourcemanager.hh>

namespace BugEngine { namespace Resource {

ResourceManager::LoaderInfo::LoaderInfo() : classinfo(), loaders(Arena::resource()), resources()
{
}

ResourceManager::ResourceManager()
    : m_loaders(Arena::resource(), 1024)
    , m_tickets(Arena::resource())
    , m_pendingTickets(Arena::resource())
    , m_watches(Arena::resource())
{
}

ResourceManager::~ResourceManager()
{
}

ResourceManager::LoaderInfo& ResourceManager::getLoaderInfo(raw< const RTTI::Class > classinfo)
{
    for(minitl::array< LoaderInfo >::iterator it = m_loaders.begin(); it != m_loaders.end(); ++it)
    {
        if(classinfo == it->classinfo) return *it;
        if(!it->classinfo)
        {
            it->classinfo = classinfo;
            return *it;
        }
    }
    be_notreached();
    return m_loaders[0];
}

void ResourceManager::attach(raw< const RTTI::Class > classinfo, weak< ILoader > loader)
{
    LoaderInfo& info = getLoaderInfo(classinfo);
    for(minitl::vector< weak< ILoader > >::iterator it = info.loaders.begin();
        it != info.loaders.end(); ++it)
    {
        be_assert_recover(*it != loader,
                          "registering twice the same loader for class %s" | classinfo->name,
                          return );
    }
    be_info("registering loader for type %s" | classinfo->name);
    info.loaders.push_back(loader);
    for(minitl::intrusive_list< const Description, 2 >::iterator resource = info.resources.begin();
        resource != info.resources.end(); ++resource)
    {
        resource->load(loader);
    }
}

void ResourceManager::detach(raw< const RTTI::Class > classinfo, weak< const ILoader > loader)
{
    for(minitl::vector< Ticket >::iterator it = m_watches.begin(); it != m_watches.end(); ++it)
    {
        if(it->loader == loader)
        {
            it->outdated = true;
            it->file     = weak< const File >();
            it->resource = weak< const Description >();
            it->loader   = weak< ILoader >();
        }
    }
    LoaderInfo& info = getLoaderInfo(classinfo);
    for(minitl::vector< weak< ILoader > >::iterator it = info.loaders.begin();
        it != info.loaders.end();)
    {
        if(*it == loader)
        {
            be_info("unregistering loader for type %s" | classinfo->name);
            for(minitl::intrusive_list< const Description, 2 >::iterator resource
                = info.resources.begin();
                resource != info.resources.end(); ++resource)
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

void ResourceManager::load(raw< const RTTI::Class >  classinfo,
                           weak< const Description > description)
{
    LoaderInfo& info = getLoaderInfo(classinfo);
    for(minitl::vector< weak< ILoader > >::const_iterator it = info.loaders.begin();
        it != info.loaders.end(); ++it)
    {
        description->load(*it);
    }
    info.resources.push_back(*description.operator->());
}

void ResourceManager::unload(raw< const RTTI::Class >  classinfo,
                             weak< const Description > description)
{
    description->unhook();
    LoaderInfo& info = getLoaderInfo(classinfo);
    for(minitl::vector< weak< ILoader > >::const_iterator it = info.loaders.begin();
        it != info.loaders.end(); ++it)
    {
        description->unload(*it);
    }

    for(minitl::vector< Ticket >::iterator it = m_watches.begin(); it != m_watches.end(); ++it)
    {
        if(it->resource == description)
        {
            it->outdated = true;
            it->file     = weak< const File >();
            it->resource = weak< const Description >();
            it->loader   = weak< ILoader >();
        }
    }
}

void ResourceManager::addTicket(weak< ILoader > loader, weak< const Description > description,
                                weak< const File > file, ILoader::FileType fileType,
                                ILoader::LoadType loadType)
{
    Ticket ticket;
    ticket.loader    = loader;
    ticket.file      = file;
    ticket.resource  = description;
    ticket.ticket    = file->beginRead(0, 0, fileType == ILoader::FileText, Arena::temporary());
    ticket.fileState = file->getState();
    ticket.progress  = 0;
    ticket.fileType  = fileType;
    ticket.loadType  = loadType;
    ticket.outdated  = false;
    m_pendingTickets.push_back(ticket);
}

size_t ResourceManager::updateTickets()
{
    size_t ticketCount = m_tickets.size();
    do
    {
        m_tickets.push_back(m_pendingTickets.begin(), m_pendingTickets.end());
        ticketCount += m_pendingTickets.size();
        m_pendingTickets.clear();

        for(minitl::vector< Ticket >::iterator it = m_tickets.begin(); it != m_tickets.end();
            /*nothing*/)
        {
            if(it->ticket->error)
            {
                be_error("resource loading error");
                it = m_tickets.erase(it);
            }
            else if(it->ticket->done())
            {
                it->loader->onTicketLoaded(it->resource,
                                           it->resource->getResourceForWriting(it->loader),
                                           it->ticket->buffer, it->loadType);
                it->ticket = ref< const File::Ticket >();
                m_watches.push_back(*it);
                it = m_tickets.erase(it);
            }
            else if(it->ticket->processed != it->progress)
            {
                it->progress = it->ticket->processed;
                it->loader->onTicketUpdated(it->resource,
                                            it->resource->getResourceForWriting(it->loader),
                                            it->ticket->buffer, it->progress, it->loadType);
                ++it;
            }
            else
            {
                ++it;
            }
        }
    } while(m_pendingTickets.size() > 0);

    minitl::vector< Ticket > updatedTickets(Arena::temporary());
    for(minitl::vector< Ticket >::iterator it = m_watches.begin(); it != m_watches.end();
        /*nothing*/)
    {
        if(it->outdated)
        {
            it = m_watches.erase(it);
        }
        else if(it->file->isDeleted())
        {
            be_info("todo: file deleted, remove resource");
            it = m_watches.erase(it);
        }
        else if(it->file->getState() != it->fileState)
        {
            updatedTickets.push_back(*it);
            it = m_watches.erase(it);
        }
        else
        {
            ++it;
        }
    }

    for(minitl::vector< Ticket >::iterator it = updatedTickets.begin(); it != updatedTickets.end();
        ++it)
    {
        it->loader->reload(it->resource, it->resource,
                           it->resource->getResourceForWriting(it->loader));
    }

    return ticketCount;
}

}}  // namespace BugEngine::Resource
