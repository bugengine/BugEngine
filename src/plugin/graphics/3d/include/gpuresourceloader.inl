/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_3D_RENDERER_GPURESOURCELOADER_INL_
#define BE_3D_RENDERER_GPURESOURCELOADER_INL_
/**************************************************************************************************/
#include <bugengine/plugin.graphics.3d/stdafx.h>
#include <bugengine/plugin.graphics.3d/renderer/igpuresource.hh>
#include <gpuresourceloader.hh>

namespace BugEngine {

template < typename R >
GPUResourceLoader< R >::GPUResourceLoader(weak< const IRenderer > renderer)
    : m_renderer(renderer)
    , m_deleted(Arena::resource())
{
}

template < typename R >
GPUResourceLoader< R >::~GPUResourceLoader()
{
}

template < typename R >
void GPUResourceLoader< R >::load(weak< const Resource::Description > description,
                                  Resource::Resource&                 resource)
{
    ref< IGPUResource > handle = m_renderer->create(be_checked_cast< const R >(description));
    resource.setRefHandle(handle);
    m_pending.push_back(*handle.operator->());
}

template < typename R >
void GPUResourceLoader< R >::reload(weak< const Resource::Description > /*oldDescription*/,
                                    weak< const Resource::Description > newDescription,
                                    Resource::Resource&                 resource)
{
    unload(resource);
    load(newDescription, resource);
}

template < typename R >
void GPUResourceLoader< R >::unload(Resource::Resource& resource)
{
    weak< IGPUResource > gpuResource = resource.getRefHandle< IGPUResource >();
    gpuResource->m_resource.clear();
    gpuResource->addref();
    m_deleted.push_back(gpuResource);
    resource.clearRefHandle();
}

template < typename R >
void GPUResourceLoader< R >::flush()
{
    while(!m_deleted.empty())
    {
        IGPUResource* resource = m_deleted.back().operator->();
        m_deleted.pop_back();
        resource->unload();
        resource->decref();
    }
    for(minitl::intrusive_list< IGPUResource >::iterator it = m_pending.begin();
        it != m_pending.end();)
    {
        IGPUResource& resource = *it;
        resource.load(resource.m_resource);
        it = m_pending.erase(it);
        m_resources.push_back(resource);
    }
}

}  // namespace BugEngine

/**************************************************************************************************/
#endif
