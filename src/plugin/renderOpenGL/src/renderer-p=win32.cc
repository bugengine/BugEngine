/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <renderer.hh>
#include    <window.hh>

#include    <GL/gl3.h>
#include    <GL/glext.h>
#include    <GL/wglext.h>

namespace BugEngine
{
    extern HINSTANCE hDllInstance;
}


namespace BugEngine { namespace Graphics { namespace OpenGL
{

class Renderer::Context : public minitl::refcountable
{
    friend class Renderer;
private:
    HGLRC   m_glContext;
public:
    Context();
    ~Context();
};

Renderer::Context::Context()
:   m_glContext(0)
{
}

Renderer::Context::~Context()
{
    if (m_glContext)
        wglDeleteContext(m_glContext);
}


class Window::Context : public minitl::refcountable
{
    friend class Renderer;
    friend class Window;
private:
    HGLRC       m_glContext;
    HDC         m_dc;
public:
    Context();
    ~Context();
};

Window::Context::Context()
:   m_glContext(0)
,   m_dc(0)
{
}

Window::Context::~Context()
{
    /*if (m_glContext)
    {
        wglDeleteContext(m_glContext);
        m_glContext = 0;
    }*/
}

//------------------------------------------------------------------------

Renderer::Renderer(weak<const FileSystem> filesystem)
    :   m_context(scoped<Context>::create(gameArena()))
    ,   m_filesystem(filesystem)
{
}

Renderer::~Renderer()
{
}

void Renderer::attachWindow(Window* w)
{
    HDC hDC = GetDC(*(HWND*)w->getWindowHandle());
    static const PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32,
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24,
        0,
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };
    GLuint pixelFormat = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, pixelFormat, &pfd);
    if (!m_context->m_glContext)
    {
        m_context->m_glContext = wglCreateContext(hDC);
    }
    w->m_context->m_dc = hDC;
    w->m_context->m_glContext = m_context->m_glContext;
    //wglShareLists(m_glContext, w->m_glContext);
}

void Renderer::createContext(void*)
{
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
    if (!wglMakeCurrent(m_context->m_dc, m_context->m_glContext))
    {
        char *errorMessage;
        ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            ::GetLastError(),
            0,
            reinterpret_cast<LPTSTR>(&errorMessage),
            0,
            NULL);
        be_error(errorMessage);
        ::LocalFree(errorMessage);
    }
}

void Window::clearCurrent()
{
    if (!wglMakeCurrent(0, 0))
    {
        char *errorMessage;
        ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            ::GetLastError(),
            0,
            reinterpret_cast<LPTSTR>(&errorMessage),
            0,
            NULL);
        be_error(errorMessage);
        ::LocalFree(errorMessage);
    }
}

void Window::present()
{
    SwapBuffers(m_context->m_dc);
}

}}}
