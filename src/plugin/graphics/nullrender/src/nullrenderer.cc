/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
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

    static int frames = 0;
    static float now = Timer::now();
    be_forceuse(now);
    if (++frames%10000 == 0)
    {
        float time = Timer::now();
        be_info("FPS: %d" | (int)(1000.0f*10000.0f / (time - now)));
        now = time;
    }
}

NullRenderer::NullRenderer(const PluginContext& context)
:   IRenderer(Arena::general(), context.resourceManager)
{
}

NullRenderer::~NullRenderer()
{
}

ref<IGPUResource> NullRenderer::create(weak<const RenderSurface> rendersurface) const
{
    return ref<NullSurface>::create(m_allocator, rendersurface, this);
}

ref<IGPUResource> NullRenderer::create(weak<const RenderWindow> renderwindow) const
{
    return ref<NullWindow>::create(m_allocator, renderwindow, this);
}

ref<IGPUResource> NullRenderer::create(weak<const ShaderProgram> shader) const
{
    return ref<NullShaderProgram>::create(m_allocator, shader, this);
}

}}
