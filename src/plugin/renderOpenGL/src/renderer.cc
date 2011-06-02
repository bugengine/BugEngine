/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <renderer.hh>
#include    <graphics/objects/rendertarget.script.hh>
#include    <loaders/rendertarget/glrendertarget.hh>
#include    <loaders/rendertarget/glwindow.hh>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

void Renderer::flush()
{
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
