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

template< typename Owner, typename T > class ResourceLoader;

class be_api(SYSTEM) Resource : public minitl::refcountable
{
    friend struct ResourceLoaders;
    BE_NOCOPY(Resource);
private:
    enum { MaxResourceCount = 4 };
    mutable minitl::pair<weak<pointer>, ResourceHandle> m_handles[MaxResourceCount];
protected:
    Resource();
    ~Resource();
private:
    void load(weak<const IResourceLoader> loader) const;
    void unload(weak<const IResourceLoader> loader) const;
public:
    static void load(const Value& value);
    static void unload(const Value& value);
    const ResourceHandle& getResource(weak<const minitl::pointer> owner) const;
};

struct be_api(SYSTEM) ResourceLoaders
{
    friend class Resource;
public:
    ResourceLoaders();
    ~ResourceLoaders();

    template< typename T, typename Owner >
    static void attach(weak<Owner> owner, ResourceHandle(Owner::*load)(weak<const T> t), void (Owner::*unload)(const ResourceHandle& handle))
    {
        Value v = be_typeid<T>::klass()->getTag(be_typeid<ResourceLoaders>::type());
        be_assert_recover(v, "type %s has no ResourceLoaders tag; no loader can be attached" |  be_typeid<T>::type().name(), return);
        v.as<ResourceLoaders&>().add(ref< ResourceLoader<Owner, T> >::create(rttiArena(), owner, load, unload));
    }

    template< typename T, typename Owner >
    static void detach(weak<Owner> owner)
    {
        Value v = be_typeid<T>::klass()->getTag(be_typeid<ResourceLoaders>::type());
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
