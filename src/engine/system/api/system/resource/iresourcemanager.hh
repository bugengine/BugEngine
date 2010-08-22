/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_RESOURCE_RESOURCEMANAGER_HH_
#define BE_SYSTEM_RESOURCE_RESOURCEMANAGER_HH_
/*****************************************************************************/

namespace BugEngine
{

class IResourceManager : public minitl::pointer
{
public:
    virtual ~IResourceManager() = 0;
    virtual void unloadResource(const void* handle) const = 0;
};

}

/*****************************************************************************/
#endif
