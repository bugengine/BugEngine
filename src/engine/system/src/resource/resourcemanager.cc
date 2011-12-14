/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/resource/resourcemanager.hh>

namespace BugEngine
{

ResourceManager::ResourceManager()
    :   m_loaders(gameArena())
{
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::attach(raw<const RTTI::ClassInfo> classinfo, weak<IResourceLoader> loader)
{
    for (minitl::vector<LoaderInfo>::iterator it = m_loaders.begin(); it != m_loaders.end(); )
    {
        if (it->loader == loader)
        {
            it = m_loaders.erase(it);
        }
        else
        {
            ++it;
        }
    }
    LoaderInfo loaderInfo;
    loaderInfo.classinfo = classinfo;
    loaderInfo.loader = loader;
    m_loaders.push_back(loaderInfo);
}

void ResourceManager::detach(raw<const RTTI::ClassInfo> classinfo, weak<const IResourceLoader> loader)
{
    for (minitl::vector<LoaderInfo>::iterator it = m_loaders.begin(); it != m_loaders.end(); )
    {
        if (it->classinfo == classinfo)
        {
            it = m_loaders.erase(it);
        }
        else
        {
            ++it;
        }
    }
    be_error("loader was not in the list of loaders for type %s" | classinfo->name);
}

void ResourceManager::load(raw<const RTTI::ClassInfo> classinfo, weak<const Resource> resource) const
{
    for (minitl::vector<LoaderInfo>::const_iterator it = m_loaders.begin(); it != m_loaders.end(); ++it)
    {
        if (it->classinfo == classinfo)
        {
            resource->load(it->loader);
        }
    }
}

void ResourceManager::unload(raw<const RTTI::ClassInfo> classinfo, weak<const Resource> resource) const
{
    for (minitl::vector<LoaderInfo>::const_iterator it = m_loaders.begin(); it != m_loaders.end(); ++it)
    {
        if (it->classinfo == classinfo)
        {
            resource->unload(it->loader);
        }
    }
}

}
