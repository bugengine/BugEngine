/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <GL4/stdafx.h>
#include    <GL4/glrenderer.hh>
#include    <3d/rendertarget/rendertarget.script.hh>
#include    <3d/shader/shader.script.hh>
#include    <loaders/rendertarget/glsurface.hh>
#include    <loaders/rendertarget/glwindow.hh>
#include    <loaders/shader/glshader.hh>

namespace BugEngine { namespace OpenGL
{

void GLRenderer::flush()
{
    Windowing::Renderer::flush();
}

ref<IGPUResource> GLRenderer::create(weak<const RenderSurfaceDescription> /*renderSurfaceDescription*/) const
{
    return ref<GLSurface>();
}

ref<IGPUResource> GLRenderer::create(weak<const RenderWindowDescription> renderWindowDescription) const
{
    return ref<GLWindow>::create(m_allocator, renderWindowDescription, this);
}

ref<IGPUResource> GLRenderer::create(weak<const ShaderProgramDescription> shaderDescription) const
{
    return ref<GLShaderProgram>::create(m_allocator, shaderDescription, this);
}

}}
