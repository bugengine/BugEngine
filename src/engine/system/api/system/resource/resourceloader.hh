/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_RESOURCE_RESOURCELOADER_HH_
#define BE_SYSTEM_RESOURCE_RESOURCELOADER_HH_
/*****************************************************************************/
#include    <system/resource/resource.script.hh>

namespace BugEngine
{

class Resource;

class be_api(SYSTEM) IResourceLoader : public minitl::refcountable
{
    friend struct ResourceLoaders;
    friend class Resource;
    BE_NOCOPY(IResourceLoader);
protected:
    const weak<minitl::pointer> m_loader;
protected:
    IResourceLoader(weak<minitl::pointer> loader);
    ~IResourceLoader();

    virtual ResourceHandle load(weak<const Resource> resource) const = 0;
    virtual void unload(const ResourceHandle& resource) const = 0;
};

template< typename Owner, typename R >
class ResourceLoader : public IResourceLoader
{
    friend struct ResourceLoaders;
    BE_NOCOPY(ResourceLoader);
private:
    typedef ResourceHandle (Owner::*LoadMethod)(weak<const R> r);
    typedef void (Owner::*UnloadMethod)(const ResourceHandle& resource);
    const LoadMethod    m_load;
    const UnloadMethod  m_unload;
public:
    weak<Owner> owner() const;
    ResourceLoader(weak<Owner> owner, LoadMethod load, UnloadMethod unload);
    ~ResourceLoader();
private:
    ResourceHandle load(weak<const Resource> resource) const  override;
    void unload(const ResourceHandle& resource) const override;
};

}

#include    <system/resource/resourceloader.inl>

/*****************************************************************************/
#endif
