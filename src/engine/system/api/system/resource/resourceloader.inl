/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_RESOURCE_RESOURCELOADER_INL_
#define BE_SYSTEM_RESOURCE_RESOURCELOADER_INL_
/*****************************************************************************/

namespace BugEngine
{

template< typename Owner, typename R >
ResourceLoader<Owner, R>::ResourceLoader(weak<Owner> owner, LoadMethod load, UnloadMethod unload)
    :   IResourceLoader(owner)
    ,   m_load(load)
    ,   m_unload(unload)
{
}

template< typename Owner, typename R >
ResourceLoader<Owner, R>::~ResourceLoader()
{
}

template< typename Owner, typename R >
weak<Owner> ResourceLoader<Owner, R>::owner() const
{
    return be_checked_cast<Owner>(m_loader);
}

template< typename Owner, typename R >
ResourceHandle ResourceLoader<Owner, R>::load(weak<const Resource> resource) const
{
    return (owner().operator->()->*m_load)(be_checked_cast<const R>(resource));
}

template< typename Owner, typename R >
void ResourceLoader<Owner, R>::unload(const ResourceHandle& resource) const
{
    (owner().operator->()->*m_unload)(resource);
}

}

/*****************************************************************************/
#endif

