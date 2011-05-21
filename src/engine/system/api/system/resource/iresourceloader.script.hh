/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_RESOURCE_IRESOURCELOADER_HH_
#define BE_SYSTEM_RESOURCE_IRESOURCELOADER_HH_
/*****************************************************************************/
#include    <system/resource/resource.script.hh>

namespace BugEngine
{

class Resource;
struct ResourceHandle;

class be_api(SYSTEM) IResourceLoader : public minitl::refcountable
{
    friend class Resource;
public:
    IResourceLoader();
    virtual ~IResourceLoader();

    template< typename T > void attach()
    {
        Value v = be_typeid<T>::klass()->template getTag<ResourceLoaders>();
        be_assert_recover(v, "type %s has no ResourceLoaders tag; no loader can be attached" |  be_typeid<T>::type().name(), return);
        v.as<ResourceLoaders&>().add(this);
    }
    template< typename T > void detach()
    {
        Value v = be_typeid<T>::klass()->template getTag<ResourceLoaders>();
        be_assert_recover(v, "type %s has no ResourceLoaders tag; no loader can be detached" |  be_typeid<T>::type().name(), return);
        v.as<ResourceLoaders&>().remove(this);
    }

public:
    virtual void* load(weak<const Resource> source) = 0;
    virtual void  unload(const void* resource) = 0;
};

}

/*****************************************************************************/
#endif

