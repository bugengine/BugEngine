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
    ,   m_renderTargetLoader(scoped< GPUResourceLoader<RenderTarget> >::create(gameArena(), this))
    ,   m_renderWindowLoader(scoped< GPUResourceLoader<RenderWindow> >::create(gameArena(), this))
    ,   m_shaderProgramLoader(scoped< GPUResourceLoader<ShaderProgram> >::create(gameArena(), this))
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

void IRenderer::flush()
{
    m_renderTargetLoader->flush();
    m_renderWindowLoader->flush();
    m_shaderProgramLoader->flush();
}

weak<IGPUResource> IRenderer::getRenderTarget(weak<const Resource> resource) const
{
    return be_checked_cast<IGPUResource>(resource->getResourceHandle(m_renderTargetLoader).handle);
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
