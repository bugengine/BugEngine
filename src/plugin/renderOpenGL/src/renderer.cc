/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <renderer.hh>

#include    <input/inputmap.hh>

#include    <window.hh>

#ifdef BE_PLATFORM_MACOS
# include   <OpenGL/OpenGL.h>
# include   <OpenGL/gl.h>
#else
# include   <GL/gl.h>
# include   <GL/gl3.h>
#endif

namespace BugEngine { namespace Graphics { namespace OpenGL
{

ref<IRenderTarget> Renderer::createRenderWindow(WindowFlags flags)
{
    return ref<Window>::create(gameArena(), this, flags);
}

ref<IRenderTarget> Renderer::createRenderBuffer(TextureFlags /*flags*/)
{
    return ref<Window>();
}

ref<IRenderTarget> Renderer::createMultipleRenderBuffer(TextureFlags /*flags*/, size_t /*count*/)
{
    return ref<Window>();
}

void Renderer::flush()
{
    Windowing::Renderer::flush();
}

}}}
