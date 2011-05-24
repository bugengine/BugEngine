/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/resource/resource.script.hh>
#include    <system/resource/iresourceloader.script.hh>

namespace BugEngine
{

Resource::Resource()
{
}

Resource::~Resource()
{
}

void Resource::load(weak<IResourceLoader> loader) const
{
    ResourceHandle* handle = 0;
    for(int i = 0; i < MaxResourceCount; ++i)
    {
        if (m_handles[i].owner == loader)
        {
            be_warning("Resource already loaded; skipping");
            return;
        }
        else if(m_handles[i].owner == 0)
        {
            handle = &m_handles[i];
        }
    }
    if (handle)
    {
        handle->owner = loader;
        handle->resource = loader->load(this);
    }
    else
    {
        be_error("Can't load resource: handles are all used");
    }
}

void Resource::unload(weak<IResourceLoader> loader) const
{
    for(int i = 0; i < MaxResourceCount; ++i)
    {
        if (m_handles[i].owner == loader)
        {
            m_handles[i].owner->unload(m_handles[i].resource);
            m_handles[i].owner = 0;
            m_handles[i].resource = 0;
            return;
        }
    }
}

void* Resource::getResource(weak<IResourceLoader> loader) const
{
    for(int i = 0; i < MaxResourceCount; ++i)
    {
        if (m_handles[i].owner == loader)
        {
            return m_handles[i].resource;
        }
    }
    return 0;
}

void Resource::load(const Value& v)
{
    be_assert_recover(be_typeid<const Resource>::type() <= v.type(), "Not a resource to load, skipping", return);
    const Resource& resource = v.as<const Resource&>();
    Value resourceloaders = v.type().metaclass->getTag<ResourceLoaders>();
    be_assert_recover(resourceloaders, "No resource loader on type %s" | v.type().name(), return);
    resourceloaders.as<const ResourceLoaders&>().load(resource);
}

void Resource::unload(const Value& v)
{
    be_assert_recover(be_typeid<const Resource>::type() <= v.type(), "Not a resource to load, skipping", return);
    const Resource& resource = v.as<const Resource&>();
    Value resourceloaders = v.type().metaclass->getTag<ResourceLoaders>();
    be_assert_recover(resourceloaders, "No resource loader on type %s" | v.type().name(), return);
    resourceloaders.as<const ResourceLoaders&>().unload(resource);
}

ResourceLoaders::ResourceLoaders()
    :   loaders(rttiArena())
{
}

ResourceLoaders::~ResourceLoaders()
{
}

void ResourceLoaders::add(weak<IResourceLoader> loader)
{
    for(minitl::vector< weak<IResourceLoader> >::const_iterator it = loaders.begin(); it != loaders.end(); ++it)
    {
        if (*it == loader)
        {
            be_error("Added loader twice");
            return;
        }
    }
    loaders.push_back(loader);
}

void ResourceLoaders::remove(weak<IResourceLoader> loader)
{
    for(minitl::vector< weak<IResourceLoader> >::iterator it = loaders.begin(); it != loaders.end(); ++it)
    {
        if (*it == loader)
        {
            loaders.erase(it);
            return;
        }
    }
    be_error("Loader was not in the list of loaders for this type");
}

void ResourceLoaders::load(const Resource& resource) const
{
    for(minitl::vector< weak<IResourceLoader> >::const_iterator it = loaders.begin(); it != loaders.end(); ++it)
    {
        resource.load(*it);
    }
}

void ResourceLoaders::unload(const Resource& resource) const
{
    for(minitl::vector< weak<IResourceLoader> >::const_iterator it = loaders.begin(); it != loaders.end(); ++it)
    {
        resource.unload(*it);
    }
}

}

