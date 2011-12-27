/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <3d/stdafx.h>
#include    <3d/renderer/irenderer.hh>
#include    <system/scheduler/task/method.hh>

#include    <3d/mesh/mesh.script.hh>
#include    <3d/shader/shader.script.hh>
#include    <3d/texture/texture.script.hh>
#include    <3d/rendertarget/rendertarget.script.hh>
#include    <gpuresourceloader.hh>
#include    <graph/loader.hh>

namespace BugEngine { namespace Graphics
{

IRenderer::IRenderer(Allocator& allocator, weak<ResourceManager> manager, Scheduler::Affinity affinity)
    :   m_allocator(allocator)
    ,   m_resourceManager(manager)
    ,   m_syncTask(ref< Task< MethodCaller<IRenderer, &IRenderer::flush> > >::create(taskArena(), "flush", color32(255,0,0),  MethodCaller<IRenderer, &IRenderer::flush>(this), Scheduler::High, affinity))
    ,   m_sceneLoader(scoped<SceneGraphLoader>::create(gameArena(), this))
    ,   m_renderSurfaceLoader(scoped< GPUResourceLoader<RenderSurface> >::create(gameArena(), this))
    ,   m_renderWindowLoader(scoped< GPUResourceLoader<RenderWindow> >::create(gameArena(), this))
    ,   m_shaderProgramLoader(scoped< GPUResourceLoader<ShaderProgram> >::create(gameArena(), this))
{
    m_resourceManager->attach<RenderNode>(m_sceneLoader);
    m_resourceManager->attach<RenderSurface>(m_renderSurfaceLoader);
    m_resourceManager->attach<RenderWindow>(m_renderWindowLoader);
    m_resourceManager->attach<ShaderProgram>(m_shaderProgramLoader);
}

IRenderer::~IRenderer()
{
    m_resourceManager->detach<ShaderProgram>(m_shaderProgramLoader);
    m_resourceManager->detach<RenderWindow>(m_renderWindowLoader);
    m_resourceManager->detach<RenderSurface>(m_renderSurfaceLoader);
    m_resourceManager->detach<RenderNode>(m_sceneLoader);
}

weak<ITask> IRenderer::syncTask() const
{
    return m_syncTask;
}

Allocator& IRenderer::arena() const
{
    return m_allocator;
}

void IRenderer::flush()
{
    m_renderSurfaceLoader->flush();
    m_renderWindowLoader->flush();
    m_shaderProgramLoader->flush();
}

weak<IGPUResource> IRenderer::getRenderSurface(weak<const Resource> resource) const
{
    return be_checked_cast<IGPUResource>(resource->getResourceHandle(m_renderSurfaceLoader).handle);
}

weak<IGPUResource> IRenderer::getRenderWindow(weak<const Resource> resource) const
{
    return be_checked_cast<IGPUResource>(resource->getResourceHandle(m_renderWindowLoader).handle);
}

weak<IGPUResource> IRenderer::getShaderProgram(weak<const Resource> resource) const
{
    return be_checked_cast<IGPUResource>(resource->getResourceHandle(m_shaderProgramLoader).handle);
}

}}
