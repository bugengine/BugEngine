/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/plugin.graphics.GL4/stdafx.h>
#include <loaders/rendertarget/glwindow.hh>

namespace BugEngine { namespace OpenGL {

void GLWindow::begin(ClearMode clear) const
{
    if(m_context)
    {
        setCurrent();
        if(clear == IRenderTarget::Clear)
        {
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        }
    }
    be_info("blabla");
}

void GLWindow::end(PresentMode presentMode) const
{
    if(m_context)
    {
        glFlush();
        if(presentMode == Present)
        {
            present();
        }
        clearCurrent();
    }
    be_info("blabla");
}

}}  // namespace BugEngine::OpenGL
