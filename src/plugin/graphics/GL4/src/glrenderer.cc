/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <glrenderer.hh>
#include    <3d/rendertarget/rendertarget.script.hh>
#include    <3d/shader/shader.script.hh>
#include    <loaders/rendertarget/glsurface.hh>
#include    <loaders/rendertarget/glwindow.hh>
#include    <loaders/shader/glshader.hh>
#include    <core/timer.hh>

namespace BugEngine { namespace OpenGL
{

void GLRenderer::flush()
{
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
