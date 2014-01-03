/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <stdafx.h>
#include    <nullrenderer.hh>

#include    <3d/rendertarget/rendertarget.script.hh>
#include    <3d/shader/shader.script.hh>
#include    <loaders/nullsurface.hh>
#include    <loaders/nullwindow.hh>
#include    <loaders/nullshader.hh>

namespace BugEngine { namespace Null
{

void NullRenderer::flush()
{
    IRenderer::flush();
}

NullRenderer::NullRenderer(const Plugin::Context& context)
:   IRenderer(Arena::general(), context.resourceManager)
{
}

NullRenderer::~NullRenderer()
{
}

ref<IGPUResource> NullRenderer::create(weak<const RenderSurfaceDescription> renderSurfaceDescription) const
{
    return ref<NullSurface>::create(m_allocator, renderSurfaceDescription, this);
}

ref<IGPUResource> NullRenderer::create(weak<const RenderWindowDescription> renderWindowDescription) const
{
    return ref<NullWindow>::create(m_allocator, renderWindowDescription, this);
}

ref<IGPUResource> NullRenderer::create(weak<const ShaderProgramDescription> shaderDescription) const
{
    return ref<NullShaderProgram>::create(m_allocator, shaderDescription, this);
}

}}
