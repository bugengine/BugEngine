/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
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

ref<IGPUResource> GLRenderer::create(weak<const RenderSurface> /*rendersurface*/) const
{
    return ref<GLSurface>();
}

ref<IGPUResource> GLRenderer::create(weak<const RenderWindow> renderwindow) const
{
    return ref<GLWindow>::create(m_allocator, renderwindow, this);
}

ref<IGPUResource> GLRenderer::create(weak<const ShaderProgram> shader) const
{
    return ref<GLShaderProgram>::create(m_allocator, shader, this);
}

}}
