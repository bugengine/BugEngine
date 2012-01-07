/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <glrenderer.hh>
#include    <extensions.hh>
#include    <loaders/rendertarget/glsurface.hh>
#include    <3d/rendertarget/rendertarget.script.hh>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

GLSurface::GLSurface(weak<const RenderSurface> resource, weak<GLRenderer> renderer)
    :   IRenderTarget(resource, renderer)
{
}

GLSurface::~GLSurface()
{
}

void GLSurface::load(weak<const Resource> /*resource*/)
{
}

void GLSurface::unload()
{
}

void GLSurface::begin(ClearMode clear) const
{
    setCurrent();
    if (clear == IRenderTarget::Clear)
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

void GLSurface::end(PresentMode presentMode) const
{
    glFlush();
    if (presentMode == Present)
    {
        present();
    }
    clearCurrent();
}


void GLSurface::setCurrent() const
{
}

void GLSurface::clearCurrent() const
{
}

void GLSurface::present() const
{
}

}}}
