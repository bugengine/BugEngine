/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_RESOURCE_RESOURCELOADER_HH_
#define BE_SYSTEM_RESOURCE_RESOURCELOADER_HH_
/*****************************************************************************/
#include    <system/resource/resource.script.hh>

namespace BugEngine
{

class Resource;
struct ResourceHandle;
struct ResourceLoaders;

class be_api(SYSTEM) IResourceLoader : public minitl::refcountable
{
    friend struct ResourceLoaders;
    friend class Resource;
protected:
    const weak<minitl::pointer> m_loader;
protected:
    IResourceLoader(weak<minitl::pointer> loader);
    ~IResourceLoader();

    virtual void* load(weak<const Resource> resource) const = 0;
    virtual void unload(const void* resource) const = 0;
};

template< typename Owner, typename R >
class ResourceLoader : public IResourceLoader
{
    friend struct ResourceLoaders;
private:
    typedef void* (Owner::*LoadMethod)(weak<const R> r);
    typedef void (Owner::*UnloadMethod)(const void* resource);
    const LoadMethod    m_load;
    const UnloadMethod  m_unload;
public:
    weak<Owner> owner() const;
    ResourceLoader(weak<Owner> owner, LoadMethod load, UnloadMethod unload);
    ~ResourceLoader();
private:
    void* load(weak<const Resource> resource) const  override;
    void unload(const void* resource) const override;
};

}

#include    <system/resource/resourceloader.inl>

/*****************************************************************************/
#endif

