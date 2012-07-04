/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <3d/stdafx.h>
#include    <3d/renderer/irenderer.hh>
#include    <system/scheduler/task/method.hh>
#include    <system/scheduler/kernel/kernel.script.hh>
#include    <3d/mesh/mesh.script.hh>
#include    <3d/shader/shader.script.hh>
#include    <3d/texture/texture.script.hh>
#include    <3d/rendertarget/rendertarget.script.hh>
#include    <gpuresourceloader.hh>
#include    <graph/loader.hh>

namespace BugEngine
{

IRenderer::IRenderer(Allocator& allocator, weak<ResourceManager> manager, Scheduler::Affinity affinity)
    :   m_allocator(allocator)
    ,   m_resourceManager(manager)
    ,   m_syncTask(ref< Task::Task< Task::MethodCaller<IRenderer, &IRenderer::flush> > >::create(Arena::task(), "flush", color32(255,0,0),  Task::MethodCaller<IRenderer, &IRenderer::flush>(this), Scheduler::High, affinity))
    ,   m_sceneLoader(scoped<SceneGraphLoader>::create(Arena::resource(), this))
    ,   m_renderSurfaceLoader(scoped< GPUResourceLoader<RenderSurface> >::create(Arena::resource(), this))
    ,   m_renderWindowLoader(scoped< GPUResourceLoader<RenderWindow> >::create(Arena::resource(), this))
    ,   m_shaderProgramLoader(scoped< GPUResourceLoader<ShaderProgram> >::create(Arena::resource(), this))
    ,   m_kernelSort(scoped<Kernel::Kernel>::create(Arena::task(), "graphics.3d.batchsort"))
    ,   m_kernelRender(scoped<Kernel::Kernel>::create(Arena::task(), "graphics.3d.batchrender"))
{
    m_resourceManager->attach(be_typeid<RenderNode>::klass(), m_sceneLoader);
    m_resourceManager->attach(be_typeid<RenderSurface>::klass(), m_renderSurfaceLoader);
    m_resourceManager->attach(be_typeid<RenderWindow>::klass(), m_renderWindowLoader);
    m_resourceManager->attach(be_typeid<ShaderProgram>::klass(), m_shaderProgramLoader);
    m_resourceManager->load(weak<Kernel::Kernel>(m_kernelSort));
    m_resourceManager->load(weak<Kernel::Kernel>(m_kernelRender));
}

IRenderer::~IRenderer()
{
    m_resourceManager->unload(weak<Kernel::Kernel>(m_kernelRender));
    m_resourceManager->unload(weak<Kernel::Kernel>(m_kernelSort));
    m_resourceManager->detach(be_typeid<ShaderProgram>::klass(), m_shaderProgramLoader);
    m_resourceManager->detach(be_typeid<RenderWindow>::klass(), m_renderWindowLoader);
    m_resourceManager->detach(be_typeid<RenderSurface>::klass(), m_renderSurfaceLoader);
    m_resourceManager->detach(be_typeid<RenderNode>::klass(), m_sceneLoader);
}

weak<Task::ITask> IRenderer::syncTask() const
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

}
