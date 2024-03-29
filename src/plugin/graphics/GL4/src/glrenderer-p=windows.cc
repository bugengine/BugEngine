/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.graphics.GL4/stdafx.h>
#include <bugengine/plugin.graphics.GL4/glmemoryhost.hh>
#include <bugengine/plugin.graphics.GL4/glrenderer.hh>
#include <extensions.hh>

#include <bugengine/core/threads/thread.hh>
#include <bugengine/plugin.graphics.3d/mesh/mesh.script.hh>
#include <bugengine/plugin.graphics.3d/rendertarget/rendertarget.script.hh>
#include <bugengine/plugin.graphics.3d/shader/shader.script.hh>
#include <bugengine/plugin.graphics.3d/texture/texture.script.hh>
#include <loaders/mesh/glmesh.hh>
#include <loaders/rendertarget/glsurface.hh>
#include <loaders/rendertarget/glwindow.hh>
#include <loaders/shader/glshader.hh>
#include <loaders/texture/gltexture.hh>

#include <GL4/wglext.h>

namespace BugEngine { namespace OpenGL {

static const PIXELFORMATDESCRIPTOR s_pfd
    = {sizeof(PIXELFORMATDESCRIPTOR),
       1,
       PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
       PFD_TYPE_RGBA,
       32,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       24,
       0,
       0,
       PFD_MAIN_PLANE,
       0,
       0,
       0,
       0};

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
    const ShaderExtensions shaderext;

public:
    Context(weak< const GLRenderer > renderer);
    ~Context();
};

static HWND createDummyWnd(weak< const GLRenderer > renderer)
{
    minitl::format< 128u > classname = minitl::format< 128u >("__be__%p__") | (const void*)renderer;
    HWND hWnd = CreateWindowEx(0, classname.c_str(), "", WS_POPUP, 0, 0, 1, 1, 0, 0,
                               (HINSTANCE)::GetModuleHandle(0), 0);
    if(!hWnd)
    {
        char* errorMessage;
        ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
                        ::GetLastError(), 0, reinterpret_cast< LPTSTR >(&errorMessage), 0, NULL);
        be_error(errorMessage);
        ::LocalFree(errorMessage);
    }
    return hWnd;
}

static HGLRC createGLContext(weak< const GLRenderer > renderer, HDC hdc)
{
    GLuint pixelFormat = ChoosePixelFormat(hdc, &s_pfd);
    SetPixelFormat(hdc, pixelFormat, &s_pfd);

    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;

    {
        HWND   hwnd = createDummyWnd(renderer);
        HDC    dc   = GetDC(hwnd);
        GLuint pf   = ChoosePixelFormat(hdc, &s_pfd);
        SetPixelFormat(dc, pf, &s_pfd);

        // phony context to get the context creation method
        HGLRC glrc = wglCreateContext(dc);
        wglMakeCurrent(dc, glrc);

        wglCreateContextAttribsARB = be_function_cast< PFNWGLCREATECONTEXTATTRIBSARBPROC >(
            wglGetProcAddress("wglCreateContextAttribsARB"));

        wglMakeCurrent(0, 0);
        wglDeleteContext(glrc);
        ReleaseDC(hwnd, dc);
        DestroyWindow(hwnd);
    }
    HGLRC rc = 0;
    if(wglCreateContextAttribsARB)
    {
        int attribs[] = {WGL_CONTEXT_MAJOR_VERSION_ARB,
                         4,
                         WGL_CONTEXT_MINOR_VERSION_ARB,
                         1,
                         WGL_CONTEXT_PROFILE_MASK_ARB,
                         WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                         WGL_CONTEXT_FLAGS_ARB,
                         WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
                         0};
        rc            = wglCreateContextAttribsARB(hdc, 0, attribs);
        if(!rc)
        {
            attribs[1] = 3;
            attribs[3] = 2;
            rc         = wglCreateContextAttribsARB(hdc, 0, attribs);
        }
        if(!rc)
        {
            attribs[1] = 3;
            attribs[3] = 0;
            rc         = wglCreateContextAttribsARB(hdc, 0, attribs);
        }
        if(!rc)
        {
            attribs[1] = 2;
            attribs[3] = 0;
            rc         = wglCreateContextAttribsARB(hdc, 0, attribs);
        }
    }

    if(!rc)
    {
        rc = wglCreateContext(hdc);
    }

    if(rc)
    {
        wglMakeCurrent(hdc, rc);
        be_info("Created OpenGL context %s (%s) on %s" | (const char*)glGetString(GL_VERSION)
                | (const char*)glGetString(GL_VENDOR) | (const char*)glGetString(GL_RENDERER));
    }
    else
    {
        be_error("Could not create a GL context");
    }
    return rc;
}

GLRenderer::Context::Context(weak< const GLRenderer > renderer)
    : m_dummyHwnd(createDummyWnd(renderer))
    , m_dummyDC(GetDC(m_dummyHwnd))
    , m_glContext(createGLContext(renderer, m_dummyDC))
    , m_setSwapInterval(
          be_function_cast< PFNWGLSWAPINTERVALEXTPROC >(wglGetProcAddress("wglSwapIntervalEXT")))
    , m_threadId(Thread::currentId())
    , shaderext()
{
}

GLRenderer::Context::~Context()
{
    wglMakeCurrent(0, 0);
    if(m_glContext) wglDeleteContext(m_glContext);
    ReleaseDC(m_dummyHwnd, m_dummyDC);
    DestroyWindow(m_dummyHwnd);
}

class GLWindow::Context : public minitl::refcountable
{
    friend class GLWindow;
    BE_NOCOPY(Context);

private:
    HGLRC m_glContext;
    HWND  m_hwnd;
    HDC   m_dc;
    HDC   m_defaultDc;
    u64   m_threadId;

public:
    Context(HGLRC rc, HWND hwnd, HDC defaultDc, u64 threadId);
    ~Context();
};

GLWindow::Context::Context(HGLRC rc, HWND hwnd, HDC defaultDc, u64 threadId)
    : m_glContext(rc)
    , m_hwnd(hwnd)
    , m_dc(GetDC(hwnd))
    , m_defaultDc(defaultDc)
    , m_threadId(threadId)
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

GLRenderer::GLRenderer(const Plugin::Context& context)
    : Windowing::Renderer(Arena::general(), context.resourceManager)
    , m_context(scoped< Context >::create(Arena::general(), this))
    , m_openGLMemoryHost(scoped< GLMemoryHost >::create(Arena::general()))
    , m_openCLScheduler("plugin.compute.opencl_gl", context)
{
}

GLRenderer::~GLRenderer()
{
    flush();
}

void GLRenderer::attachWindow(weak< GLWindow > w) const
{
    be_assert(Thread::currentId() == m_context->m_threadId, "render command on wrong thread");
    HWND wnd = *(HWND*)w->getWindowHandle();
    w->m_context.reset(scoped< GLWindow::Context >::create(Arena::general(), m_context->m_glContext,
                                                           wnd, m_context->m_dummyDC,
                                                           m_context->m_threadId));
    w->setCurrent();
    if(m_context->m_setSwapInterval) (*m_context->m_setSwapInterval)(0);
    w->clearCurrent();
}

const ShaderExtensions& GLRenderer::shaderext() const
{
    be_assert(m_context, "extensions required before context was created");
    return m_context->shaderext;
}

bool GLRenderer::success() const
{
    return Windowing::Renderer::success() && (m_context != 0);
}

//------------------------------------------------------------------------

GLWindow::GLWindow(weak< const RenderWindowDescription > renderwindow,
                   weak< const GLRenderer >              renderer)
    : Windowing::Window(renderwindow, renderer)
    , m_context(scoped< Context >())
{
}

GLWindow::~GLWindow()
{
}

void GLWindow::load(weak< const Resource::Description > description)
{
    Window::load(description);
    be_checked_cast< const GLRenderer >(m_renderer)->attachWindow(this);
}

void GLWindow::unload()
{
    be_assert(Thread::currentId() == m_context->m_threadId, "render command on wrong thread");
    Window::unload();
    m_context.reset(scoped< Context >());
}

void GLWindow::setCurrent() const
{
    be_assert(Thread::currentId() == m_context->m_threadId, "render command on wrong thread");
    if(!wglMakeCurrent(m_context->m_dc, m_context->m_glContext))
    {
        char* errorMessage;
        ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
                        ::GetLastError(), 0, reinterpret_cast< LPTSTR >(&errorMessage), 0, NULL);
        be_error(errorMessage);
        ::LocalFree(errorMessage);
    }
}

void GLWindow::clearCurrent() const
{
    be_assert(Thread::currentId() == m_context->m_threadId, "render command on wrong thread");
    if(!wglMakeCurrent(m_context->m_defaultDc, m_context->m_glContext))
    {
        char* errorMessage;
        ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
                        ::GetLastError(), 0, reinterpret_cast< LPTSTR >(&errorMessage), 0, NULL);
        be_error(errorMessage);
        ::LocalFree(errorMessage);
    }
}

void GLWindow::present() const
{
    be_assert(Thread::currentId() == m_context->m_threadId, "render command on wrong thread");
    SwapBuffers(m_context->m_dc);
}

}}  // namespace BugEngine::OpenGL
