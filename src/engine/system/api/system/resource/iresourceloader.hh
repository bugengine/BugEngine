/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_RESOURCE_IRESOURCELOADER_HH_
#define BE_SYSTEM_RESOURCE_IRESOURCELOADER_HH_
/*****************************************************************************/

namespace BugEngine
{

class Resource;
class ResourceHandle;

class IResourceLoader : public minitl::refcountable
{
public:
    IResourceLoader();
protected:
    ~IResourceLoader();

public:
    virtual void* load(weak<const Resource> source) const = 0;
    virtual void  unload(const void* resource) const = 0;
};

}

/*****************************************************************************/
#endif

