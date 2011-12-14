/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_RESOURCE_RESOURCELOADER_HH_
#define BE_SYSTEM_RESOURCE_RESOURCELOADER_HH_
/*****************************************************************************/
#include    <system/resource/resource.script.hh>

namespace BugEngine
{

class ResourceManager;

class be_api(SYSTEM) IResourceLoader : public minitl::refcountable
{
    friend class ResourceManager;
    friend class Resource;
    BE_NOCOPY(IResourceLoader);
protected:
    IResourceLoader();
    ~IResourceLoader();

    virtual ResourceHandle load(weak<const Resource> resource) = 0;
    virtual void unload(const ResourceHandle& resource) = 0;
};

}

/*****************************************************************************/
#endif
