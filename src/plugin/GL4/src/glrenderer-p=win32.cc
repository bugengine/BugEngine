/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <glrenderer.hh>
#include    <extensions.hh>

#include    <3d/rendertarget/rendertarget.script.hh>
#include    <3d/mesh/mesh.script.hh>
#include    <3d/texture/texture.script.hh>
#include    <3d/shader/shader.script.hh>
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

class GLRenderer::Context : public minitl::refcountable
{
    friend class GLRenderer;
    BE_NOCOPY(Context);
private:
    HWND                            m_dummyHwnd;
    HDC                             m_dummyDC;
    HGLRC                           m_glContext;
    const PFNWGLSWAPINTERVALEXTPROC m_setSwapInterval;
    u64                             m_threadId;
public:
    const ShaderExtensions  shaderext;
public:
    Context(weak<const GLRenderer> renderer);
    ~Context();
};

static HWND createDummyWnd(weak<const GLRenderer> renderer)
{
    minitl::format<> classname = minitl::format<>("__be__%p__") | (const void*)renderer;
    HWND hWnd = CreateWindowEx( 0, classname.c_str(), "", WS_POPUP, 0, 0, 1, 1, 0, 0, hDllInstance, 0);
    if (!hWnd)
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
    return hWnd;
}

static HGLRC createGLContext(weak<const GLRenderer> renderer, HDC hdc)
{
    GLuint pixelFormat = ChoosePixelFormat(hdc, &s_pfd);
    SetPixelFormat(hdc, pixelFormat, &s_pfd);

    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;

    {
        HWND hwnd = createDummyWnd(renderer);
        HDC dc = GetDC(hwnd);
        GLuint pixelFormat = ChoosePixelFormat(hdc, &s_pfd);
        SetPixelFormat(dc, pixelFormat, &s_pfd);

        // phony context to get the context creation method
        HGLRC glrc = wglCreateContext(dc);
        wglMakeCurrent(dc, glrc);

        wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

        wglMakeCurrent(0, 0);
        wglDeleteContext(glrc);
        ReleaseDC(hwnd, dc);
        DestroyWindow(hwnd);
    }
    HGLRC rc = 0;
    if (wglCreateContextAttribsARB)
    {
        int attribs[] =
                {
                    WGL_CONTEXT_MAJOR_VERSION_ARB,  4,
                    WGL_CONTEXT_MINOR_VERSION_ARB,  1,
                    WGL_CONTEXT_PROFILE_MASK_ARB,   WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                    WGL_CONTEXT_FLAGS_ARB,          WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
                    0
                };
        rc = wglCreateContextAttribsARB(hdc, 0, attribs);
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
    }

    if(!rc)
    {
        rc = wglCreateContext(hdc);
    }

    if(rc)
    {
        wglMakeCurrent(hdc, rc);
        be_info("Created OpenGL context %s (%s) on %s" | (const char*)glGetString(GL_VERSION) | (const char *)glGetString(GL_VENDOR) | (const char*)glGetString(GL_RENDERER));
    }
    else
    {
        be_error("Could not create a GL context");
    }
    return rc;
}

GLRenderer::Context::Context(weak<const GLRenderer> renderer)
:   m_dummyHwnd(createDummyWnd(renderer))
,   m_dummyDC(GetDC(m_dummyHwnd))
,   m_glContext(createGLContext(renderer, m_dummyDC))
,   m_setSwapInterval((PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT"))
,   m_threadId(Thread::currentId())
,   shaderext()
{
}

GLRenderer::Context::~Context()
{
    wglMakeCurrent(0, 0);
    if (m_glContext)
        wglDeleteContext(m_glContext);
    ReleaseDC(m_dummyHwnd, m_dummyDC);
    DestroyWindow(m_dummyHwnd);
}


class GLWindow::Context : public minitl::refcountable
{
    friend class GLWindow;
    BE_NOCOPY(Context);
private:
    HGLRC       m_glContext;
    HWND        m_hwnd;
    HDC         m_dc;
    HDC         m_defaultDc;
    u64         m_threadId;
public:
    Context(HGLRC rc, HWND hwnd, HDC defaultDc, u64 threadId);
    ~Context();
};

GLWindow::Context::Context(HGLRC rc, HWND hwnd, HDC defaultDc, u64 threadId)
:   m_glContext(rc)
,   m_hwnd(hwnd)
,   m_dc(GetDC(hwnd))
,   m_defaultDc(defaultDc)
,   m_threadId(threadId)
{
    GLuint pixelFormat = ChoosePixelFormat(m_dc, &s_pfd);
    SetPixelFormat(m_dc, pixelFormat, &s_pfd);
}

GLWindow::Context::~Context()
{
    ReleaseDC(m_hwnd, m_dc);
    /*if (m_glContext)
    {
        wglDeleteContext(m_glContext);
        m_glContext = 0;
    }*/
}

//------------------------------------------------------------------------

GLRenderer::GLRenderer(weak<ResourceManager> manager)
    :   Windowing::Renderer(gameArena(), manager)
    ,   m_context(scoped<Context>::create(gameArena(), this))
{
}

GLRenderer::~GLRenderer()
{
}

void GLRenderer::attachWindow(weak<GLWindow> w) const
{
    be_assert(Thread::currentId() == m_context->m_threadId, "render command on wrong thread");
    HWND wnd = *(HWND*)w->getWindowHandle();
    w->m_context = scoped<GLWindow::Context>::create(arena(), m_context->m_glContext, wnd, m_context->m_dummyDC, m_context->m_threadId);
    w->setCurrent();
    if (m_context->m_setSwapInterval)
        (*m_context->m_setSwapInterval)(0);
    w->clearCurrent();
}

const ShaderExtensions& GLRenderer::shaderext() const
{
    be_assert(m_context, "extensions required before context was created");
    return m_context->shaderext;
}

//------------------------------------------------------------------------

GLWindow::GLWindow(weak<const RenderWindow> renderwindow, weak<GLRenderer> renderer)
:   Windowing::Window(renderwindow, renderer)
,   m_context(scoped<Context>())
{
}

GLWindow::~GLWindow()
{
}

void GLWindow::load(weak<const Resource> resource)
{
    Window::load(resource);
    be_checked_cast<const GLRenderer>(m_renderer)->attachWindow(this);
}

void GLWindow::unload()
{
    be_assert(Thread::currentId() == m_context->m_threadId, "render command on wrong thread");
    Window::unload();
    m_context = scoped<Context>();
}

void GLWindow::setCurrent() const
{
    be_assert(Thread::currentId() == m_context->m_threadId, "render command on wrong thread");
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

void GLWindow::clearCurrent() const
{
    be_assert(Thread::currentId() == m_context->m_threadId, "render command on wrong thread");
    if (!wglMakeCurrent(m_context->m_defaultDc, m_context->m_glContext))
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

void GLWindow::present() const
{
    be_assert(Thread::currentId() == m_context->m_threadId, "render command on wrong thread");
    SwapBuffers(m_context->m_dc);
}

}}}
