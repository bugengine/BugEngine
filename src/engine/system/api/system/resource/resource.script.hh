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

class be_api(SYSTEM) Resource : public minitl::refcountable
                              , public minitl::intrusive_list<Resource>::item
{
    friend class IResourceLoader;
private:
    enum { MaxResourceCount = 4 };
    mutable ResourceHandle  m_handles[MaxResourceCount];
protected:
    Resource();
    ~Resource();
public:
    void load(weak<IResourceLoader> loader) const;
    void unload(weak<IResourceLoader> loader) const;
    void* getResource(weak<IResourceLoader> owner) const;

    static void load(const Value& v);
    static void unload(const Value& v);
};

struct be_api(SYSTEM) ResourceLoaders
{
    ResourceLoaders();
    ~ResourceLoaders();
    minitl::vector< weak<IResourceLoader> > loaders;

    void add(weak<IResourceLoader> loader);
    void remove(weak<IResourceLoader> loader);
    void load(const Resource& resource) const;
    void unload(const Resource& resource) const;
};

}

#include    <system/resource/iresourceloader.script.hh>

/*****************************************************************************/
#endif
