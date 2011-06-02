/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <renderer.hh>
#include    <extensions.hh>

#include    <graphics/objects/rendertarget.script.hh>
#include    <graphics/objects/mesh.script.hh>
#include    <graphics/objects/texture.script.hh>
#include    <graphics/objects/shader.script.hh>
#include    <loaders/rendertarget/glrendertarget.hh>
#include    <loaders/rendertarget/glwindow.hh>
#include    <loaders/mesh/glmesh.hh>
#include    <loaders/texture/gltexture.hh>
#include    <loaders/shader/glshader.hh>


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
    const ShaderExtensions  shaderext;
public:
    Context(HDC dc);
    ~Context();
};

static HGLRC createGLContext(HDC dc)
{
    HGLRC rc = wglCreateContext(dc);
    wglMakeCurrent(dc, rc);
    return rc;
}

Renderer::Context::Context(HDC dc)
:   m_glContext(createGLContext(dc))
,   shaderext()
{
    wglMakeCurrent(dc, 0);
}

Renderer::Context::~Context()
{
    if (m_glContext)
        wglDeleteContext(m_glContext);
}


class GLWindow::Context : public minitl::refcountable
{
    friend class Renderer;
    friend class GLWindow;
private:
    HGLRC       m_glContext;
    HDC         m_dc;
public:
    Context();
    ~Context();
};

GLWindow::Context::Context()
:   m_glContext(0)
,   m_dc(0)
{
}

GLWindow::Context::~Context()
{
    /*if (m_glContext)
    {
        wglDeleteContext(m_glContext);
        m_glContext = 0;
    }*/
}

//------------------------------------------------------------------------

Renderer::Renderer(weak<const FileSystem> filesystem)
    :   Windowing::Renderer(gameArena())
    ,   m_context()
    ,   m_filesystem(filesystem)
{
}

Renderer::~Renderer()
{
}

void Renderer::attachWindow(GLWindow* w) const
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
    if (!m_context)
    {
        m_context = scoped<Context>::create(arena(), hDC);
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

const ShaderExtensions& Renderer::shaderext() const
{
    be_assert(m_context, "extensions required before context was created");
    return m_context->shaderext;
}

//------------------------------------------------------------------------

GLWindow::GLWindow(weak<const RenderWindow> renderwindow, weak<Renderer> renderer)
:   Windowing::Window(renderwindow, renderer)
{
}

GLWindow::~GLWindow()
{
}

void GLWindow::load(weak<const Resource> resource)
{
    Window::load(resource);
    m_context = scoped<Context>::create(m_renderer->arena());
    be_checked_cast<const Renderer>(m_renderer)->attachWindow(this);
}

void GLWindow::unload()
{
    Window::unload();
    m_context = scoped<Context>();
}

void GLWindow::setCurrent()
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

void GLWindow::clearCurrent()
{
    if (!wglMakeCurrent(m_context->m_dc, NULL))
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

void GLWindow::present()
{
    SwapBuffers(m_context->m_dc);
}

}}}
