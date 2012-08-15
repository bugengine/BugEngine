/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_RESOURCE_RESOURCEMANAGER_HH_
#define BE_RESOURCE_RESOURCEMANAGER_HH_
/*****************************************************************************/
#include    <minitl/refcountable.hh>
#include    <minitl/intrusive_list.hh>
#include    <resource/resource.script.hh>
#include    <filesystem/file.script.hh>

namespace BugEngine
{

class be_api(RESOURCE) ResourceManager : public minitl::pointer
{
    BE_NOCOPY(ResourceManager);
private:
    struct LoaderInfo
    {
        LoaderInfo();
        raw<const RTTI::Class>                      classinfo;
        minitl::vector< weak<IResourceLoader> >     loaders;
        minitl::intrusive_list<const Resource, 2>   resources;
    };
    struct Ticket
    {
        weak<IResourceLoader>   loader;
        weak<const Resource>    resource;
        ref<const File::Ticket> ticket;
        u32                     progress;
    };
private:
    minitl::array<LoaderInfo>   m_loaders;
    minitl::vector<Ticket>      m_tickets;
private:
    LoaderInfo& getLoaderInfo(raw<const RTTI::Class> classinfo);
public:
    ResourceManager();
    ~ResourceManager();

    void attach(raw<const RTTI::Class> classinfo, weak<IResourceLoader> loader);
    void detach(raw<const RTTI::Class> classinfo, weak<const IResourceLoader> loader);
    void load(raw<const RTTI::Class> classinfo, weak<const Resource> resource);
    void unload(raw<const RTTI::Class> classinfo, weak<const Resource> resource);

    template< typename T > void attach(weak<IResourceLoader> loader)        { attach(be_typeid<T>::klass(), loader); }
    template< typename T > void detach(weak<const IResourceLoader> loader)  { detach(be_typeid<T>::klass(), loader); }
    template< typename T > void load(weak<T> resource)                      { load(be_typeid<T>::klass(), resource); }
    template< typename T > void load(ref<T> resource)                       { load(be_typeid<T>::klass(), resource); }
    template< typename T > void unload(weak<T> resource)                    { unload(be_typeid<T>::klass(), resource); }
    template< typename T > void unload(ref<T> resource)                     { unload(be_typeid<T>::klass(), resource); }

    void addTicket(weak<IResourceLoader> loader, weak<const Resource> resource, weak<const File> file);
    size_t updateTickets();
};

}

/*****************************************************************************/
#endif
