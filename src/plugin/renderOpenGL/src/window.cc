/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <window.hh>
#include    <renderer.hh>
#ifdef BE_PLATFORM_MACOS
# include   <OpenGL/OpenGL.h>
# include   <OpenGL/gl.h>
#else
# include   <GL/gl.h>
# include   <GL/gl3.h>
#endif


namespace BugEngine { namespace Graphics { namespace OpenGL
{

void Window::begin(ClearMode clear)
{
    setCurrent();
    if (clear == IRenderTarget::Clear)
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

void Window::end(PresentMode presentMode)
{
    glFlush();
    if(presentMode == Present)
    {
        present();
    }
    clearCurrent();
}

}}}
