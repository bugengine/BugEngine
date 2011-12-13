/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <nullrenderer.hh>

#include    <3d/rendertarget/rendertarget.script.hh>
#include    <3d/shader/shader.script.hh>
#include    <loaders/nullrendertarget.hh>
#include    <loaders/nullwindow.hh>
#include    <loaders/nullshader.hh>

namespace BugEngine { namespace Graphics { namespace Null
{

void NullRenderer::flush()
{
    IRenderer::flush();

    static int frames = 0;
    static float now = Timer::now();
    be_forceuse(now);
    if (++frames%100 == 0)
    {
        float time = Timer::now();
        be_debug("FPS: %d" | (int)(1000.0f*100.0f / (time - now)));
        now = time;
    }
}

NullRenderer::NullRenderer(weak<ResourceManager> manager)
:   IRenderer(gameArena(), manager)
{
}

NullRenderer::~NullRenderer()
{
}

ref<IGPUResource> NullRenderer::createRenderTarget(weak<const RenderTarget> rendertarget) const
{
    return ref<NullRenderTarget>::create(m_allocator, rendertarget, this);
}

ref<IGPUResource> NullRenderer::createRenderWindow(weak<const RenderWindow> renderwindow) const
{
    return ref<NullWindow>::create(m_allocator, renderwindow, this);
}

ref<IGPUResource> NullRenderer::createShaderProgram(weak<const ShaderProgram> shader) const
{
    return ref<NullShaderProgram>::create(m_allocator, shader, this);
}

}}}

