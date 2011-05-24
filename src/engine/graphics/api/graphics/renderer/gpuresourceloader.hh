/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_RENDERER_IGPULOADER_HH_
#define BE_GRAPHICS_RENDERER_IGPULOADER_HH_
/*****************************************************************************/
#include    <system/resource/iresourceloader.script.hh>
#include    <graphics/renderer/irenderer.hh>
#include    <graphics/renderer/igpuresource.hh>

namespace BugEngine { namespace Graphics
{

template< typename SystemResource, typename GPUResource >
class GPUResourceLoader : public IResourceLoader
{
protected:
    const weak<IRenderer>               m_renderer;
    minitl::vector< ref<GPUResource> >  m_resources;
    minitl::vector< ref<GPUResource> >  m_pendingDelete;
    minitl::istack< GPUResource> >      m_toLoad;
public:
    GPUResourceLoader(weak<IRenderer> renderer)
        :   m_renderer(renderer)
        ,   m_resources(renderer->arena())
        ,   m_toLoad()
    {
        attach<SystemResource>();
    }

    ~GPUResourceLoader()
    {
        detach<SystemResource>();
    }

    virtual void* load(weak<const Resource> source) override
    {
        GPUResource resource = ref<GPUResource>::create(m_renderer->arena(), m_renderer, source);
        resource->m_index = be_checked_numcast<i32>(m_resources.size());
        m_resources.push_back(resource);
        return resource.operator->();
    }
    virtual void  unload(const void* resource) override
    {
        weak<const GPUResource> gpuresource((const GPUResource*) resource);
        be_assert_recover(resource->m_index >= 0, "invalid resource index", return);
        be_assert_recover(resource->m_index < m_resources.size(), "invalid resource index", return);
        be_assert_recover(m_resources[resource->m_index] == gpuresource, "invalid resource index", return);
        m_pendingDelete.append(m_resources[resource->m_index]);
        m_resources.back()->m_index = resource->m_index;
        m_resources[resource->m_index] = m_resources.back();
        m_resources.pop_back();
    }
};

}}

/*****************************************************************************/
#endif
