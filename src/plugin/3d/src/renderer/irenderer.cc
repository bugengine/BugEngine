/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <3d/stdafx.h>
#include    <3d/renderer/irenderer.hh>
#include    <system/scheduler/task/method.hh>

#include    <3d/mesh/mesh.script.hh>
#include    <3d/shader/shader.script.hh>
#include    <3d/texture/texture.script.hh>
#include    <3d/rendertarget/rendertarget.script.hh>
#include    <graph/loader.hh>

namespace BugEngine { namespace Graphics
{

IRenderer::IRenderer(Allocator& allocator, weak<ResourceManager> manager, Scheduler::Affinity affinity)
    :   m_allocator(allocator)
    ,   m_resourceManager(manager)
    ,   m_sceneLoader(scoped<SceneGraphLoader>::create(gameArena(), this))
    ,   m_syncTask(ref< Task< MethodCaller<IRenderer, &IRenderer::flush> > >::create(taskArena(), "flush", color32(255,0,0),  MethodCaller<IRenderer, &IRenderer::flush>(this), Scheduler::High, affinity))
    ,   m_deletedObjects(allocator)
{
}

IRenderer::~IRenderer()
{
}

weak<ITask> IRenderer::syncTask() const
{
    return m_syncTask;
}

Allocator& IRenderer::arena() const
{
    return m_allocator;
}

ResourceHandle IRenderer::load(weak<const RenderTarget> rendertarget)
{
    ResourceHandle handle;
    ref<IGPUResource> resource = createRenderTarget(rendertarget);
    handle.handle = resource;
    m_pendingRenderTargets.push_back(*resource);
    return handle;
}

ResourceHandle IRenderer::load(weak<const RenderWindow> renderwindow)
{
    ResourceHandle handle;
    ref<IGPUResource> resource = createRenderWindow(renderwindow);
    handle.handle = resource;
    m_pendingRenderTargets.push_back(*resource);
    return handle;
}

ResourceHandle IRenderer::load(weak<const ShaderProgram> shader)
{
    ResourceHandle handle;
    ref<IGPUResource> resource = createShaderProgram(shader);
    handle.handle = resource;
    m_pendingShaders.push_back(*resource);
    return handle;
}

void IRenderer::destroy(const ResourceHandle& r)
{
    be_checked_cast<IGPUResource>(r.handle)->m_resource = 0;
    m_deletedObjects.push_back(r.handle);
}

void IRenderer::flush()
{
    m_deletedObjects.clear();
    for(minitl::intrusive_list<IGPUResource>::iterator it = m_pendingRenderTargets.begin(); it != m_pendingRenderTargets.end(); ++it)
    {
        it->load(it->m_resource);
    }
    m_pendingRenderTargets.clear();
    for(minitl::intrusive_list<IGPUResource>::iterator it = m_pendingShaders.begin(); it != m_pendingShaders.end(); ++it)
    {
        it->load(it->m_resource);
    }
    m_pendingShaders.clear();
}

}}
