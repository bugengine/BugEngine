/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <renderer.hh>
#include    <window.hh>

#include    <GL/gl.h>
#include    <GL/glext.h>
#include    <GL/glx.h>
#include    <GL/glxext.h>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

#define GLX_CONTEXT_MAJOR_VERSION_ARB       0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB       0x2092
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

class Renderer::Context : public minitl::refcountable
{
    friend class Renderer;
    friend class Window;
private:
    ::Display*  m_display;
    GLXContext  m_glContext;
public:
    Context(::Display* display, ::GLXFBConfig fbConfig);
    ~Context();
};

Renderer::Context::Context(::Display* display, ::GLXFBConfig fbConfig)
:   m_display(display)
{
    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddress((const GLubyte *)"glXCreateContextAttribsARB");
    if (glXCreateContextAttribsARB)
    {
        int attribs[] =
            {
                GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
                GLX_CONTEXT_MINOR_VERSION_ARB, 0,
                //GLX_CONTEXT_FLAGS_ARB        , GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
                None
            };
        m_glContext = glXCreateContextAttribsARB(display, fbConfig, 0, True, attribs);
        if (!m_glContext)
        {
            attribs[1] = 1;
            attribs[3] = 0;
            m_glContext = glXCreateContextAttribsARB(display, fbConfig, 0, True, attribs);
        }
        be_info("Creating OpenGL %d.%d context" | attribs[1] | attribs[3]);
    }
    else
    {
        m_glContext = glXCreateNewContext(display, fbConfig, GLX_RGBA_TYPE, 0, True);
    }
    XSync(display, false);
}

Renderer::Context::~Context()
{
}


class Window::Context : public minitl::refcountable
{
    friend class Renderer;
    friend class Window;
private:
    GLXContext  m_glContext;
public:
    Context();
    ~Context();
};

Window::Context::Context()
:   m_glContext(0)
{
}

Window::Context::~Context()
{
}

//------------------------------------------------------------------------

Renderer::Renderer(weak<const FileSystem> filesystem)
:   m_context()
,   m_filesystem(filesystem)
{
}

Renderer::~Renderer()
{
    destroyContextAsync();
}

void Renderer::attachWindow(Window* w)
{
    if (!m_context)
    {
        createContextAsync(0);
    }
    w->m_context->m_glContext = m_context->m_glContext;
}

void Renderer::createContext(void* params)
{
    const struct DisplayInfo
    {
        ::Display* display;
        ::GLXFBConfig fbConfig;
    } *p = (const DisplayInfo*)params;
    m_context = scoped<Context>::create(gameArena(), p->display, p->fbConfig);
}

void Renderer::destroyContext()
{
}

//------------------------------------------------------------------------

Window::Window(weak<Renderer> renderer, WindowFlags flags)
:   Windowing::Window(renderer, flags)
,   m_context(scoped<Context>::create(gameArena()))
{
    renderer->attachWindow(this);
}

Window::~Window()
{
}

void Window::setCurrent()
{
    if (!closed())
    {
        ::Window* w = (::Window*)getWindowHandle();
        glXMakeCurrent(be_checked_cast<Renderer>(m_renderer)->m_context->m_display, *w, be_checked_cast<Renderer>(m_renderer)->m_context->m_glContext);
    }
}

void Window::clearCurrent()
{
    if (!closed())
    {
        glXMakeCurrent(be_checked_cast<Renderer>(m_renderer)->m_context->m_display, 0, 0);
    }
}

void Window::present()
{
    if (!closed())
    {
        ::Window* w = (::Window*)getWindowHandle();
        glXSwapBuffers(be_checked_cast<Renderer>(m_renderer)->m_context->m_display, *w);
    }
}


}}}

