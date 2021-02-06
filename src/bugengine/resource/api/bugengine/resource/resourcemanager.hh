/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RESOURCE_RESOURCEMANAGER_HH_
#define BE_RESOURCE_RESOURCEMANAGER_HH_
/**************************************************************************************************/
#include <bugengine/resource/stdafx.h>
#include <bugengine/filesystem/file.script.hh>
#include <bugengine/minitl/array.hh>
#include <bugengine/minitl/intrusive_list.hh>
#include <bugengine/minitl/refcountable.hh>
#include <bugengine/resource/description.script.hh>
#include <bugengine/resource/loader.hh>
#include <bugengine/rtti/classinfo.script.hh>
#include <bugengine/rtti/typeinfo.hh>

namespace BugEngine { namespace Resource {

class be_api(RESOURCE) ResourceManager : public minitl::pointer
{
    BE_NOCOPY(ResourceManager);

private:
    struct LoaderInfo
    {
        LoaderInfo();
        raw< const RTTI::Class >                       classinfo;
        minitl::vector< weak< ILoader > >              loaders;
        minitl::intrusive_list< const Description, 2 > resources;
    };
    struct Ticket
    {
        weak< ILoader >           loader;
        weak< const Description > resource;
        weak< const File >        file;
        ref< const File::Ticket > ticket;
        u64                       fileState;
        u32                       progress;
        ILoader::FileType         fileType;
        ILoader::LoadType         loadType;
        bool                      outdated;
    };

private:
    minitl::array< LoaderInfo > m_loaders;
    minitl::vector< Ticket >    m_tickets;
    minitl::vector< Ticket >    m_pendingTickets;
    minitl::vector< Ticket >    m_watches;

private:
    LoaderInfo& getLoaderInfo(raw< const RTTI::Class > classinfo);

public:
    ResourceManager();
    ~ResourceManager();

    void attach(raw< const RTTI::Class > classinfo, weak< ILoader > loader);
    void detach(raw< const RTTI::Class > classinfo, weak< const ILoader > loader);
    void load(raw< const RTTI::Class > classinfo, weak< const Description > resource);
    void unload(raw< const RTTI::Class > classinfo, weak< const Description > resource);

    template < typename T >
    void attach(weak< ILoader > loader)
    {
        attach(be_class< T >(), loader);
    }
    template < typename T >
    void detach(weak< const ILoader > loader)
    {
        detach(be_class< T >(), loader);
    }
    template < typename T >
    void load(weak< const T > resource)
    {
        load(be_class< T >(), resource);
    }
    template < typename T >
    void load(ref< const T > resource)
    {
        load(be_class< T >(), resource);
    }
    template < typename T >
    void unload(weak< const T > resource)
    {
        unload(be_class< T >(), resource);
    }
    template < typename T >
    void unload(ref< const T > resource)
    {
        unload(be_class< T >(), resource);
    }

    void   addTicket(weak< ILoader > loader, weak< const Description > description,
                     weak< const File > file, ILoader::FileType fileType, ILoader::LoadType loadType);
    size_t updateTickets();
};

}}  // namespace BugEngine::Resource

/**************************************************************************************************/
#endif
