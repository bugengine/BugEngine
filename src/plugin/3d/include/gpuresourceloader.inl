/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_3D_RENDERER_GPURESOURCELOADER_INL_
#define BE_3D_RENDERER_GPURESOURCELOADER_INL_
/*****************************************************************************/
#include    <3d/renderer/igpuresource.hh>

namespace BugEngine { namespace Graphics
{

template< typename R >
GPUResourceLoader<R>::GPUResourceLoader(weak<const IRenderer> renderer)
    :   m_renderer(renderer)
    ,   m_deletedResources(gameArena())
{
}


template< typename R >
GPUResourceLoader<R>::~GPUResourceLoader()
{
}

template< typename R >
ResourceHandle GPUResourceLoader<R>::load(weak<const Resource> resource)
{
    ResourceHandle r;
    r.handle = m_renderer->create(be_checked_cast<const R>(resource));
    return r;
}

template< typename R >
void GPUResourceLoader<R>::unload(const ResourceHandle& handle)
{
    m_deletedResources.push_back(be_checked_cast<IGPUResource>(handle.handle));
}

template< typename R >
void GPUResourceLoader<R>::flush()
{
    m_deletedResources.clear();
}

}}

/*****************************************************************************/
#endif

