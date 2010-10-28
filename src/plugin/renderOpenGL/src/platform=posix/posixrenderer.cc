/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <renderer.hh>
#include    <window.hh>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

#define GLX_CONTEXT_MAJOR_VERSION_ARB       0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB       0x2092
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

class Renderer::Context
{
    friend class Renderer;
private:
    GLXContext  m_glContext;
public:
    Context();
    ~Context();
};

Renderer::Context::Context()
{
}

Renderer::Context::~Context()
{
    if(m_glContext)
        wglDeleteContext(m_glContext);
}

Renderer::Renderer(weak<const FileSystem> filesystem)
    :   m_context(scoped<Context>::create<Arena::General>())
    ,   m_filesystem(filesystem)
{
}

Renderer::~Renderer()
{
}

void Renderer::attachWindow(Window* w)
{
    if(!m_context->m_glContext)
    {
        glXCreateContextAttribsARBProc glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddress((const GLubyte *)"glXCreateContextAttribsARB");
        if(glXCreateContextAttribsARB)
        {
            int attribs[] =
                {
                    GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
                    GLX_CONTEXT_MINOR_VERSION_ARB, 0,
                    //GLX_CONTEXT_FLAGS_ARB        , GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
                    None
                };
            m_context->m_glContext = glXCreateContextAttribsARB(m_display, m_fbConfig, 0, True, attribs);
            if(!m_glContext)
            {
                attribs[1] = 1;
                attribs[3] = 0;
                m_glContext = glXCreateContextAttribsARB(m_display, m_fbConfig, 0, True, attribs);
            }
            be_info("Creating OpenGL %d.%d context" | attribs[1] | attribs[3]);
        }
        else
        {
            m_context->m_glContext = glXCreateNewContext(m_display, m_fbConfig, GLX_RGBA_TYPE, 0, True);
        }
        XSync(m_display, false);
    }
    w->m_glContext = m_context->m_glContext;
}

//------------------------------------------------------------------------

Window::Window(weak<Renderer> renderer, WindowFlags flags)
:   Windowing::Window(renderer, flags)
,   m_closed(0)
{
    renderer->attachWindow(this);
}

Window::~Window()
{
}

void Window::setCurrent()
{
    if(m_window)
    {
        glXMakeCurrent(be_checked_cast<Renderer>(m_renderer)->m_display, m_window, be_checked_cast<Renderer>(m_renderer)->m_glContext);
    }
}

void Window::clearCurrent()
{
    if(m_window)
    {
        glXMakeCurrent(be_checked_cast<Renderer>(m_renderer)->m_display, 0, 0);
    }
}

void Window::present()
{
    glXSwapBuffers(be_checked_cast<Renderer>(m_renderer)->m_display, m_window);
}


}}}

