/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <GL4/stdafx.h>
#include    <loaders/rendertarget/glwindow.hh>


namespace BugEngine { namespace OpenGL
{

void GLWindow::begin(ClearMode clear) const
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

void GLWindow::end(PresentMode presentMode) const
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

}}
