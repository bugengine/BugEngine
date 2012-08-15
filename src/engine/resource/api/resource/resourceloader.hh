/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_RESOURCE_RESOURCELOADER_HH_
#define BE_RESOURCE_RESOURCELOADER_HH_
/*****************************************************************************/
#include    <resource/resource.script.hh>

namespace BugEngine
{

class ResourceManager;

class be_api(RESOURCE) IResourceLoader : public minitl::refcountable
{
    friend class ResourceManager;
    friend class Resource;
    BE_NOCOPY(IResourceLoader);
protected:
    IResourceLoader();
    ~IResourceLoader();

    virtual ResourceHandle load(weak<const Resource> resource) = 0;
    virtual void unload(const ResourceHandle& resource) = 0;

    virtual void onTicketUpdated(weak<const Resource> /*resource*/, const minitl::Allocator::Block<u8>& /*buffer*/, u32 /*progress*/) { }
    virtual void onTicketLoaded(weak<const Resource> /*resource*/, const minitl::Allocator::Block<u8>& /*buffer*/) { be_notreached(); }
};

}

/*****************************************************************************/
#endif
