/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_RESOURCE_RESOURCEMANAGER_HH_
#define BE_RESOURCE_RESOURCEMANAGER_HH_
/*****************************************************************************/
#include    <minitl/refcountable.hh>
#include    <minitl/intrusive_list.hh>
#include    <resource/description.script.hh>
#include    <filesystem/file.script.hh>

namespace BugEngine { namespace Resource
{

class be_api(RESOURCE) ResourceManager : public minitl::pointer
{
    BE_NOCOPY(ResourceManager);
private:
    struct LoaderInfo
    {
        LoaderInfo();
        raw<const RTTI::Class>                          classinfo;
        minitl::vector< weak<ILoader> >                 loaders;
        minitl::intrusive_list<const Description, 2>    resources;
    };
    struct Ticket
    {
        weak<ILoader>           loader;
        weak<const Description>    resource;
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

    void attach(raw<const RTTI::Class> classinfo, weak<ILoader> loader);
    void detach(raw<const RTTI::Class> classinfo, weak<const ILoader> loader);
    void load(raw<const RTTI::Class> classinfo, weak<const Description> resource);
    void unload(raw<const RTTI::Class> classinfo, weak<const Description> resource);

    template< typename T > void attach(weak<ILoader> loader)        { attach(be_typeid<T>::klass(), loader); }
    template< typename T > void detach(weak<const ILoader> loader)  { detach(be_typeid<T>::klass(), loader); }
    template< typename T > void load(weak<T> resource)              { load(be_typeid<T>::klass(), resource); }
    template< typename T > void load(ref<T> resource)               { load(be_typeid<T>::klass(), resource); }
    template< typename T > void unload(weak<T> resource)            { unload(be_typeid<T>::klass(), resource); }
    template< typename T > void unload(ref<T> resource)             { unload(be_typeid<T>::klass(), resource); }

    void addTicket(weak<ILoader> loader, weak<const Description> description, weak<const File> file);
    size_t updateTickets();
};

}}

/*****************************************************************************/
#endif
