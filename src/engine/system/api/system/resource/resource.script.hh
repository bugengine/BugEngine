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
struct ResourceLoaders;

class be_api(SYSTEM) Resource : public minitl::refcountable
                              , public minitl::intrusive_list<Resource>::item
{
    friend struct ResourceLoaders;
private:
    enum { MaxResourceCount = 4 };
    mutable ResourceHandle  m_handles[MaxResourceCount];
protected:
    Resource();
    ~Resource();
private:
    void load(weak<const IResourceLoader> loader) const;
    void unload(weak<const IResourceLoader> loader) const;
public:
    static void load(const Value& value);
    static void unload(const Value& value);
    void* getResource(weak<const minitl::pointer> owner) const;
};

struct be_api(SYSTEM) ResourceLoaders
{
    friend class Resource;
public:
    ResourceLoaders();
    ~ResourceLoaders();

    template< typename T, typename Owner >
    static void attach(weak<Owner> owner, void*(Owner::*load)(weak<const T> t), void (Owner::*unload)(const void*))
    {
        Value v = be_typeid<T>::klass()->template getTag<ResourceLoaders>();
        be_assert_recover(v, "type %s has no ResourceLoaders tag; no loader can be attached" |  be_typeid<T>::type().name(), return);
        v.as<ResourceLoaders&>().add(ref< const ResourceLoader<Owner, T> >::create(rttiArena(), owner, load, unload));
    }

    template< typename T, typename Owner >
    static void detach(weak<Owner> owner)
    {
        Value v = be_typeid<T>::klass()->template getTag<ResourceLoaders>();
        be_assert_recover(v, "type %s has no ResourceLoaders tag; no loader can be detached" |  be_typeid<T>::type().name(), return);
        v.as<ResourceLoaders&>().remove(owner);
    }
private:
    minitl::vector< ref<const IResourceLoader> > m_loaders;

    void add(ref<const IResourceLoader> loader);
    void remove(weak<const minitl::pointer> loader);
    inline void load(const Value& resource) const;
    inline void unload(const Value& resource) const;
};

}

#include    <system/resource/resourceloader.hh>

/*****************************************************************************/
#endif
