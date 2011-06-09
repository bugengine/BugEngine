/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <nullrenderer.hh>

#include    <graphics/objects/rendertarget.script.hh>
#include    <loaders/nullrendertarget.hh>
#include    <loaders/nullwindow.hh>

namespace BugEngine { namespace Graphics { namespace Null
{

void NullRenderer::flush()
{
    IRenderer::flush();

    static int frames = 0;
    static float now = Timer::now();
    if (++frames%100 == 0)
    {
        float time = Timer::now();
        be_debug("FPS: %d" | (int)(1000.0f*100.0f / (time - now)));
        now = time;
    }
}

NullRenderer::NullRenderer(weak<const FileSystem> filesystem)
:   IRenderer(gameArena())
,   m_filesystem(filesystem)
{
}

NullRenderer::~NullRenderer()
{
}

ref<IGPUResource> NullRenderer::createRenderTarget(weak<const RenderTarget> rendertarget)
{
    return ref<NullRenderTarget>::create(m_allocator, rendertarget, this);
}

ref<IGPUResource> NullRenderer::createRenderWindow(weak<const RenderWindow> renderwindow)
{
    return ref<NullWindow>::create(m_allocator, renderwindow, this);
}

}}}

