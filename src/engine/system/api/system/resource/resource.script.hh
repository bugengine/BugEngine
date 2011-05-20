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
    void load(weak<const IResourceLoader> loader) const;
    void unload(weak<const IResourceLoader> loader) const;
    void* getResource(weak<const IResourceLoader> owner) const;
};

struct be_api(SYSTEM) ResourceLoaders
{
    ResourceLoaders();
    ~ResourceLoaders();
    minitl::vector< weak<const IResourceLoader> > loaders;

    void add(weak<const IResourceLoader> loader);
    void remove(weak<const IResourceLoader> loader);
};

}

#include    <system/resource/iresourceloader.script.hh>

/*****************************************************************************/
#endif
