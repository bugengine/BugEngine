/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_RESOURCE_RESOURCE_HH_
#define BE_SYSTEM_RESOURCE_RESOURCE_HH_
/*****************************************************************************/
#include    <system/resource/iresourcemanager.hh>

namespace BugEngine
{

class Resource
{
    friend class IResourceManager;
private:
    struct ResourceHandle
    {
    private:
        weak<const IResourceManager>    m_manager;
        const void*                     m_handle;
    public:
        ResourceHandle();
        ResourceHandle(weak<const IResourceManager> manager, const void* handle);
        ~ResourceHandle();
    };
private:
    minitl::list<ResourceHandle, Arena::General>    m_handles;
public:
    Resource();
    ~Resource();
};

}

/*****************************************************************************/
#endif
