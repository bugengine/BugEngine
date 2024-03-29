/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RESOURCE_RESOURCE_HH_
#define BE_RESOURCE_RESOURCE_HH_
/**************************************************************************************************/
#include <bugengine/resource/stdafx.h>
#include <bugengine/resource/handle.script.hh>

namespace BugEngine { namespace Resource {

struct be_api(RESOURCE) Resource
{
    friend class Description;

private:
    Handle m_handle;

private:
    Resource();
    Resource(const Resource& other);
    Resource& operator=(const Resource& other);

    minitl::refcountable* getRefHandleInternal() const;

public:
    ~Resource();

    void setRefHandle(ref< minitl::refcountable > handle);
    void clearRefHandle();
    template < typename T >
    weak< T > getRefHandle() const
    {
        return be_checked_cast< T >(getRefHandleInternal());
    }

    static Resource& null();

    operator const void*() const
    {
        return (const void*)(this - &null());
    }
    bool operator!() const
    {
        return !this->operator const void*();
    }
};

}}  // namespace BugEngine::Resource

/**************************************************************************************************/
#endif
