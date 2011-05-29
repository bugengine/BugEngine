/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_RESOURCE_RESOURCEHANDLE_HH_
#define BE_SYSTEM_RESOURCE_RESOURCEHANDLE_HH_
/*****************************************************************************/

namespace BugEngine
{

class IResourceLoader;

struct ResourceHandle
{
    weak<const minitl::pointer> owner;
    void*                       resource;

    ResourceHandle();
    ~ResourceHandle();
};

}

/*****************************************************************************/
#endif

