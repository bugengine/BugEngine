/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_RESOURCE_RESOURCE_INL_
#define BE_SYSTEM_RESOURCE_RESOURCE_INL_
/*****************************************************************************/

namespace BugEngine
{

template< typename Type, typename Pipeline>
Resource<Type,Pipeline>::Resource()
    :   m_resource(0)
    ,   m_loader(0)
{
}

template< typename Type, typename Pipeline>
Resource<Type,Pipeline>::Resource(Pipeline* loader, const ifilename& file)
    :   m_resource(0)
    ,   m_loader(0)
{
    load(loader, file);
}

template< typename Type, typename Pipeline>
Resource<Type,Pipeline>::~Resource()
{
    unload();
}

template< typename Type, typename Pipeline>
bool Resource<Type,Pipeline>::isBound() const
{
    return m_resource != 0;
}

template< typename Type, typename Pipeline>
Resource<Type,Pipeline>::operator void *() const
{
    return m_resource;
}

template< typename Type, typename Pipeline>
Type* Resource<Type,Pipeline>::get()
{
    return m_resource;
}

template< typename Type, typename Pipeline>
Type& Resource<Type,Pipeline>::operator*()
{
    return *m_resource;
}

template< typename Type, typename Pipeline>
const Type& Resource<Type,Pipeline>::operator*() const
{
    return *m_resource;
}

template< typename Type, typename Pipeline>
Type* Resource<Type,Pipeline>::operator->()
{
    return m_resource;
}

template< typename Type, typename Pipeline>
const Type* Resource<Type,Pipeline>::operator->() const
{
    return m_resource;
}

template< typename Type, typename Pipeline>
void Resource<Type,Pipeline>::load(Pipeline* loader, const ifilename& file)
{
    unload();
    m_loader = loader;
    m_resource = m_loader->load(file);
}

template< typename Type, typename Pipeline>
void Resource<Type,Pipeline>::unload()
{
    if(m_resource)
    {
        m_loader->unload(m_resource);
        m_loader = 0;
        m_resource = 0;
    }
}



}


/*****************************************************************************/
#endif
