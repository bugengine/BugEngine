/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <renderer.hh>
#include    <graphics/objects/rendertarget.script.hh>
#include    <loaders/rendertarget/glrendertarget.hh>
#include    <loaders/rendertarget/glwindow.hh>
#include    <core/timer.hh>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

void Renderer::flush()
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

ref<IGPUResource> Renderer::createRenderTarget(weak<const RenderTarget> rendertarget)
{
    return ref<GLRenderTarget>();
}

ref<IGPUResource> Renderer::createRenderWindow(weak<const RenderWindow> renderwindow)
{
    return ref<GLWindow>::create(m_allocator, renderwindow, this);
}

}}}
