/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_RESOURCE_RESOURCE_INL_
#define BE_SYSTEM_RESOURCE_RESOURCE_INL_
/*****************************************************************************/

namespace BugEngine
{

template<typename Destination, typename Source, typename ResourceLoader >
Resource<Destination, Source, ResourceLoader>::Resource(weak<const ResourceLoader> loader, const Source& file)
:   m_resource(0)
,   m_loader(loader)
{
    load(file);
}

template<typename Destination, typename Source, typename ResourceLoader>
Resource<Destination, Source, ResourceLoader>::~Resource()
{
    unload();
}

template<typename Destination, typename Source, typename ResourceLoader>
bool Resource<Destination, Source, ResourceLoader>::isBound() const
{
    return m_resource != 0;
}

template<typename Destination, typename Source, typename ResourceLoader>
Resource<Destination, Source, ResourceLoader>::operator const void *() const
{
    return m_resource;
}

template<typename Destination, typename Source, typename ResourceLoader>
bool Resource<Destination, Source, ResourceLoader>::operator!() const
{
    return !m_resource;
}

template<typename Destination, typename Source, typename ResourceLoader>
weak<Destination> Resource<Destination, Source, ResourceLoader>::get()
{
    return m_resource;
}

template<typename Destination, typename Source, typename ResourceLoader>
Destination* Resource<Destination, Source, ResourceLoader>::operator->()
{
    return m_resource.operator->();
}

template<typename Destination, typename Source, typename ResourceLoader>
const Destination* Resource<Destination, Source, ResourceLoader>::operator->() const
{
    return m_resource.operator->();
}

template<typename Destination, typename Source, typename ResourceLoader>
void Resource<Destination, Source, ResourceLoader>::load(const Source& source)
{
    unload();
    m_resource = m_loader->load(source);
}

template<typename Destination, typename Source, typename ResourceLoader>
void Resource<Destination, Source, ResourceLoader>::unload()
{
    if(m_resource)
    {
        m_loader->unload(m_resource);
        m_resource = 0;
    }
}



}


/*****************************************************************************/
#endif
