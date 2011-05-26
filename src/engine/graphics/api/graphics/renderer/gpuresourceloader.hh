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
    const weak<IRenderer>                   m_renderer;
    minitl::vector< ref<GPUResource> >      m_resources;
    minitl::vector< ref<GPUResource> >      m_pendingDelete;
    minitl::intrusive_list< IGPUResource >  m_toLoad;
public:
    GPUResourceLoader(weak<IRenderer> renderer)
        :   m_renderer(renderer)
        ,   m_resources(renderer->arena())
        ,   m_pendingDelete(renderer->arena())
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
        ref<GPUResource> resource = ref<GPUResource>::create(m_renderer->arena(), source, m_renderer);
        resource->m_index = be_checked_numcast<i32>(m_resources.size());
        m_resources.push_back(resource);
        return resource.operator->();
    }

    virtual void  unload(const void* resource) override
    {
        weak<const GPUResource> gpuresource((const GPUResource*) resource);
        be_assert_recover(gpuresource->m_index >= 0, "invalid resource index", return);
        be_assert_recover(gpuresource->m_index < m_resources.size(), "invalid resource index", return);
        be_assert_recover(m_resources[gpuresource->m_index] == gpuresource, "invalid resource index", return);
        gpuresource->m_resource = 0;
        m_pendingDelete.push_back(m_resources[gpuresource->m_index]);
        m_resources.back()->m_index = gpuresource->m_index;
        m_resources[gpuresource->m_index] = m_resources.back();
        m_resources.erase(m_resources.end()-1);
    }

    void update()
    {
        for(minitl::vector< ref<GPUResource> >::const_iterator it = m_pendingDelete.begin(); it != m_pendingDelete.end(); ++it)
        {
            (*it)->unload();
        }
        m_pendingDelete.clear();
        for(minitl::intrusive_list< IGPUResource >::iterator it = m_toLoad.begin(); it != m_toLoad.end(); ++it)
        {
            (*it)->load();
        }
        m_toLoad.clear();
    }
};

}}

/*****************************************************************************/
#endif
