/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <renderer.hh>
#include    <extensions.hh>
#include    <loaders/rendertarget/glrendertarget.hh>
#include    <graphics/objects/rendertarget.script.hh>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

GLRenderTarget::GLRenderTarget(weak<const Resource> resource, weak<Renderer> renderer)
    :   IRenderTarget(resource, renderer)
{
}

GLRenderTarget::~GLRenderTarget()
{
}

void GLRenderTarget::load(weak<const Resource> resource)
{
}

void GLRenderTarget::unload()
{
}

void GLRenderTarget::begin(ClearMode clear)
{
    setCurrent();
    if (clear == IRenderTarget::Clear)
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

void GLRenderTarget::end(PresentMode presentMode)
{
    glFlush();
    if (presentMode == Present)
    {
        present();
    }
    clearCurrent();
}


void GLRenderTarget::setCurrent()
{
}

void GLRenderTarget::clearCurrent()
{
}

void GLRenderTarget::present()
{
}

bool GLRenderTarget::closed() const
{
    return false;
}

}}}
