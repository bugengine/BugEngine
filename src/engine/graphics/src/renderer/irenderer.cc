/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/renderer/irenderer.hh>
#include    <system/scheduler/task/method.hh>

#include    <graphics/objects/mesh.script.hh>
#include    <graphics/objects/shader.script.hh>
#include    <graphics/objects/texture.script.hh>
#include    <graphics/objects/rendertarget.script.hh>
#include    <graph/loader.hh>

namespace BugEngine { namespace Graphics
{

IRenderer::IRenderer(Allocator& allocator)
    :   m_allocator(allocator)
    ,   m_sceneLoader(scoped<SceneGraphLoader>::create(gameArena(), this))
    ,   m_syncTask(ref< Task< MethodCaller<IRenderer, &IRenderer::flush> > >::create(taskArena(), "flush", color32(255,0,0),  MethodCaller<IRenderer, &IRenderer::flush>(this), Scheduler::High, Scheduler::MainThread))
    ,   m_deletedObjects(allocator)
{
    ResourceLoaders::attach<RenderTarget, IRenderer>(this, &IRenderer::load, &IRenderer::destroy);
    ResourceLoaders::attach<RenderWindow, IRenderer>(this, &IRenderer::load, &IRenderer::destroy);
    //ResourceLoaders::attach<Mesh, IRenderer>(this, &IRenderer::load, &IRenderer::destroy);
    //ResourceLoaders::attach<Texture, IRenderer>(this, &IRenderer::load, &IRenderer::destroy);
    //ResourceLoaders::attach<Shader, IRenderer>(this, &IRenderer::load, &IRenderer::destroy);
}

IRenderer::~IRenderer()
{
    //ResourceLoaders::detach<Shader, IRenderer>(this);
    //ResourceLoaders::detach<Texture, IRenderer>(this);
    //ResourceLoaders::detach<Mesh, IRenderer>(this);
    ResourceLoaders::detach<RenderWindow, IRenderer>(this);
    ResourceLoaders::detach<RenderTarget, IRenderer>(this);
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
}

}}
