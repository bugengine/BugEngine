/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_RESOURCE_RESOURCE_SCRIPT_HH_
#define BE_SYSTEM_RESOURCE_RESOURCE_SCRIPT_HH_
/*****************************************************************************/
#include    <minitl/ptr/refcountable.hh>
#include    <system/resource/resourcehandle.hh>

namespace BugEngine
{

class IResourceLoader;
class ResourceManager;

class be_api(SYSTEM) Resource : public minitl::refcountable
{
    friend class ResourceManager;
    BE_NOCOPY(Resource);
private:
    enum
    {
        MaxResourceCount = 4
    };
    mutable minitl::pair<weak<IResourceLoader>, ResourceHandle> m_handles[MaxResourceCount];
private:
    void load(weak<IResourceLoader> loader) const;
    void unload(weak<const IResourceLoader> loader) const;
protected:
    Resource();
    ~Resource();
public:
    const ResourceHandle& getResourceHandle(weak<const IResourceLoader> owner) const;
    ResourceHandle& getResourceHandleForWriting(weak<const IResourceLoader> owner) const;
};

}

#include    <system/resource/resourceloader.hh>

/*****************************************************************************/
#endif
