/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <glrenderer.hh>
#include    <graphics/objects/rendertarget.script.hh>
#include    <graphics/objects/shader.script.hh>
#include    <loaders/rendertarget/glrendertarget.hh>
#include    <loaders/rendertarget/glwindow.hh>
#include    <loaders/shader/glshader.hh>
#include    <core/timer.hh>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

void GLRenderer::flush()
{
    static int frames = 0;
    static float now = Timer::now();
    if (++frames%100 == 0)
    {
        float time = Timer::now();
        be_debug("FPS: %d" | (int)(1000.0f*100.0f / (time - now)));
        now = time;
    }

    Windowing::Renderer::flush();
}

ref<IGPUResource> GLRenderer::createRenderTarget(weak<const RenderTarget> rendertarget)
{
    return ref<GLRenderTarget>();
}

ref<IGPUResource> GLRenderer::createRenderWindow(weak<const RenderWindow> renderwindow)
{
    return ref<GLWindow>::create(m_allocator, renderwindow, this);
}

ref<IGPUResource> GLRenderer::createShaderProgram(weak<const ShaderProgram> shader)
{
    return ref<GLShaderProgram>::create(m_allocator, shader, this);
}

ref<IGPUResource> GLRenderer::createVertexShader(weak<const VertexShader> shader)
{
    return ref<GLShader>::create(m_allocator, shader, this);
}

ref<IGPUResource> GLRenderer::createGeometryShader(weak<const GeometryShader> shader)
{
    return ref<GLShader>::create(m_allocator, shader, this);
}

ref<IGPUResource> GLRenderer::createFragmentShader(weak<const FragmentShader> shader)
{
    return ref<GLShader>::create(m_allocator, shader, this);
}

}}}
