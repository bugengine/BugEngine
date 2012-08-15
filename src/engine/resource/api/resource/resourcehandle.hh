/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_RESOURCE_RESOURCEHANDLE_HH_
#define BE_RESOURCE_RESOURCEHANDLE_HH_
/*****************************************************************************/

namespace BugEngine
{

class IResourceLoader;

struct be_api(RESOURCE) ResourceHandle
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

    static ResourceHandle& null();

    inline operator const void*() const { return (const void*)(this - &null()); }
    bool operator!() const              { return this->operator const void*() == 0; }
};

}

/*****************************************************************************/
#endif

