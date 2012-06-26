/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <nullrenderer.hh>

#include    <core/timer.hh>
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
    static int frameCount = 100;
    static float now = Timer::now();
    if (++frames%frameCount == 0)
    {
        float time = Timer::now();
        float t = (time-now)/float(frameCount);
        if (t > 10.0f)
        {
            be_info("Average frame time: %d milliseconds" | (int)t);
            frameCount = 20;
        }
        else
        {
            t = 1000.0f*t;
            if (t > 10.0f)
            {
                be_info("Average frame time: %d microseconds" | (int)t);
                frameCount = 5000;
            }
            else
            {
                be_info("Average frame time: %d nanoseconds" | (int)(t*1000.0f));
                frameCount = 50000;
            }
        }
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
