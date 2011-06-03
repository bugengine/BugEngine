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

static const PIXELFORMATDESCRIPTOR s_pfd =
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

class Renderer::Context : public minitl::refcountable
{
    friend class Renderer;
private:
    HWND                            m_dummyHwnd;
    HDC                             m_dummyDC;
    HGLRC                           m_glContext;
    const PFNWGLSWAPINTERVALEXTPROC m_setSwapInterval;
public:
    const ShaderExtensions  shaderext;
public:
    Context(weak<const Renderer> renderer);
    ~Context();
};

static HWND createDummyWnd(weak<const Renderer> renderer)
{
    minitl::format<> classname = minitl::format<>("__be__%p__") | (const void*)renderer;
    HWND hWnd = CreateWindowEx( 0, classname.c_str(), "", WS_POPUP, 0, 0, 1, 1, 0, 0, hDllInstance, 0);
    return hWnd;
}

static HGLRC createGLContext(weak<const Renderer> renderer, HDC hdc)
{
    GLuint pixelFormat = ChoosePixelFormat(hdc, &s_pfd);
    SetPixelFormat(hdc, pixelFormat, &s_pfd);

    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;

    {
        HWND hwnd = createDummyWnd(renderer);
        HDC dc = GetDC(hwnd);
        GLuint pixelFormat = ChoosePixelFormat(hdc, &s_pfd);
        SetPixelFormat(dc, pixelFormat, &s_pfd);

        // phony context to 
        HGLRC glrc = wglCreateContext(dc);
        wglMakeCurrent(dc, glrc);

        wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

        wglMakeCurrent(0, 0);
        wglDeleteContext(glrc);
        ReleaseDC(hwnd, dc);
        DestroyWindow(hwnd);
    }
    int attribs[] =
            {
                WGL_CONTEXT_MAJOR_VERSION_ARB,  4,
                WGL_CONTEXT_MINOR_VERSION_ARB,  1,
                WGL_CONTEXT_PROFILE_MASK_ARB,   WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                WGL_CONTEXT_FLAGS_ARB,          WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
                0
            };
    HGLRC rc = wglCreateContextAttribsARB(hdc, 0, attribs);
    if(!rc)
    {
        attribs[1] = 3;
        attribs[3] = 2;
        rc = wglCreateContextAttribsARB(hdc, 0, attribs);
    }
    if(!rc)
    {
        attribs[1] = 3;
        attribs[3] = 0;
        rc = wglCreateContextAttribsARB(hdc, 0, attribs);
    }
    if(!rc)
    {
        attribs[1] = 2;
        attribs[3] = 0;
        rc = wglCreateContextAttribsARB(hdc, 0, attribs);
    }
    if(!rc)
    {
        be_error("Could not create a GL context");
    }
    else
    {
        wglMakeCurrent(hdc, rc);
        be_info("Created OpenGL context %s (%s)" | (const char*)glGetString(GL_VERSION) | (const char *)glGetString(GL_VENDOR));
    }
    return rc;
}

Renderer::Context::Context(weak<const Renderer> renderer)
:   m_dummyHwnd(createDummyWnd(renderer))
,   m_dummyDC(GetDC(m_dummyHwnd))
,   m_glContext(createGLContext(renderer, m_dummyDC))
,   m_setSwapInterval((PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT"))
,   shaderext()
{
    (*m_setSwapInterval)(1);
    wglMakeCurrent(0, 0);
}

Renderer::Context::~Context()
{
    wglMakeCurrent(0, 0);
    if (m_glContext)
        wglDeleteContext(m_glContext);
    ReleaseDC(m_dummyHwnd, m_dummyDC);
    DestroyWindow(m_dummyHwnd);
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
    ,   m_filesystem(filesystem)
    ,   m_context(scoped<Context>::create(gameArena(), this))
{
}

Renderer::~Renderer()
{
}

void Renderer::attachWindow(GLWindow* w) const
{
    HWND wnd = *(HWND*)w->getWindowHandle();
    HDC hDC = GetDC(wnd);
    GLuint pixelFormat = ChoosePixelFormat(hDC, &s_pfd);
    SetPixelFormat(hDC, pixelFormat, &s_pfd);
    w->m_context->m_dc = hDC;
    w->m_context->m_glContext = m_context->m_glContext;
    ReleaseDC(wnd, hDC);
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
