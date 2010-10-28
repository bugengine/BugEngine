/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <window.hh>
#include    <renderer.hh>
#include    <GL/gl.h>


namespace BugEngine { namespace Graphics { namespace OpenGL
{

bool Window::closed() const
{
    return m_closed > 0;
}

void Window::close()
{
    m_closed++;
}

void Window::begin(ClearMode clear)
{
    if(!isClosed())
    {
        setCurrent();
        if (clear == IRenderTarget::Clear)
        {
            glClearColor(0.3f, 0.3f, 03.f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        }
    }
}

void Window::end(PresentMode presentMode)
{
    if(!isClosed())
    {
        glFlush();
        if(presentMode == Present)
        {
            present();
        }
        clearCurrent();
        if(m_closed > 0 && !isClosed())
        {
            Windowing::Window::close();
            /*if(m_glContext)
            {
                wglDeleteContext(m_glContext);
                m_glContext = 0;
            }*/
        }
    }
}

}}}
