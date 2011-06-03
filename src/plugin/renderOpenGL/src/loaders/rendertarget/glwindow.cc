/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <loaders/rendertarget/glwindow.hh>
#include    <renderer.hh>


namespace BugEngine { namespace Graphics { namespace OpenGL
{

void GLWindow::begin(ClearMode clear)
{
    if (m_context)
    {
        setCurrent();
        if (clear == IRenderTarget::Clear)
        {
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        }
    }
}

void GLWindow::end(PresentMode presentMode)
{
    if (m_context)
    {
        glFlush();
        if (presentMode == Present)
        {
            present();
        }
        clearCurrent();
    }
}

}}}
