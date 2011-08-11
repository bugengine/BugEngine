/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_RESOURCE_RESOURCEHANDLE_HH_
#define BE_SYSTEM_RESOURCE_RESOURCEHANDLE_HH_
/*****************************************************************************/

namespace BugEngine
{

class IResourceLoader;

struct be_api(SYSTEM) ResourceHandle
{
    union Id
    {
        void*   ptrId;
        u32     intId;
    };
    ref<minitl::refcountable>   handle;
    mutable Id                  cache;

    ResourceHandle();
    ~ResourceHandle();

    static const ResourceHandle& null();

    inline operator const void*() const { return (const void*)(this - &null()); }
    bool operator!() const              { return this->operator const void*() == 0; }
};

}

/*****************************************************************************/
#endif

