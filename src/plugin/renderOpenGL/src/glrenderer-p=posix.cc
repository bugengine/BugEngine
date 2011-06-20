/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <glrenderer.hh>
#include    <extensions.hh>


#include    <graphics/objects/rendertarget.script.hh>
#include    <graphics/objects/mesh.script.hh>
#include    <graphics/objects/texture.script.hh>
#include    <graphics/objects/shader.script.hh>
#include    <loaders/rendertarget/glwindow.hh>
#include    <loaders/mesh/glmesh.hh>
#include    <loaders/texture/gltexture.hh>
#include    <loaders/shader/glshader.hh>


#include    <GL/glx.h>
#include    <GL/glxext.h>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

#define GLX_CONTEXT_MAJOR_VERSION_ARB       0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB       0x2092
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

struct PlatformData
{
    ::Display*      display;
    ::GLXFBConfig   fbConfig;
    ::XVisualInfo*  visual;
};

class GLRenderer::Context : public minitl::refcountable
{
    friend class GLRenderer;
    friend class GLWindow;
private:
    ::Display*  m_display;
    ::Window    m_defaultWindow;
    GLXContext  m_glContext;
    u64         m_threadId;
public:
    const ShaderExtensions  shaderext;
public:
    Context(PlatformData* data);
    ~Context();
};

static GLXContext createGLXContext(::Display* display, ::GLXFBConfig fbConfig)
{
    GLXContext context = 0;
    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddress((const GLubyte *)"glXCreateContextAttribsARB");
    if (glXCreateContextAttribsARB)
    {
        int attribs[] =
            {
                GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
                GLX_CONTEXT_MINOR_VERSION_ARB, 1,
                //GLX_CONTEXT_FLAGS_ARB        , GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
                None
            };
        context = glXCreateContextAttribsARB(display, fbConfig, 0, True, attribs);
        if (!context)
        {
            attribs[1] = 2;
            attribs[3] = 0;
            context = glXCreateContextAttribsARB(display, fbConfig, 0, True, attribs);
        }
        if (!context)
        {
            attribs[1] = 1;
            attribs[3] = 0;
            context = glXCreateContextAttribsARB(display, fbConfig, 0, True, attribs);
        }
    }
    else
    {
        context = glXCreateNewContext(display, fbConfig, GLX_RGBA_TYPE, 0, True);
    }
    return context;
}

static ::Window createDefaultWindow(::Display* display, ::XVisualInfo* visual)
{
    XSetWindowAttributes attributes;
    attributes.colormap = XCreateColormap(display, XRootWindow(display, visual->screen), visual->visual, AllocNone);
    attributes.border_pixel = 0;
    attributes.override_redirect = false; //flags.fullscreen
    attributes.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask;
    int attributeMask = CWBorderPixel | CWEventMask | CWOverrideRedirect | CWColormap;
    ::Window result = XCreateWindow(display,
                                    XRootWindow(display, visual->screen),
                                    1, 1,
                                    1, 1,
                                    1,
                                    visual->depth,
                                    InputOutput,
                                    visual->visual,
                                    attributeMask,
                                    &attributes);
    XSync(display, false);
    return result;
}


GLRenderer::Context::Context(PlatformData* data)
:   m_display(data->display)
,   m_glContext(createGLXContext(data->display, data->fbConfig))
,   m_defaultWindow(createDefaultWindow(m_display, data->visual))
,   m_threadId(Thread::currentId())
,   shaderext()
{
    glXMakeCurrent(m_display, m_defaultWindow, m_glContext);
    be_info("Creating OpenGL %s (%s) on %s" | (const char*)glGetString(GL_VERSION) | (const char *)glGetString(GL_VENDOR) | (const char*)glGetString(GL_RENDERER));
}

GLRenderer::Context::~Context()
{
    XDestroyWindow(m_display, m_defaultWindow);
}


class GLWindow::Context : public minitl::refcountable
{
    friend class GLRenderer;
    friend class GLWindow;
private:
    GLXContext  m_glContext;
    u64         m_threadId;
public:
    Context(GLXContext context, u64 threadId);
    ~Context();
};

GLWindow::Context::Context(GLXContext context, u64 threadId)
:   m_glContext(context)
,   m_threadId(threadId)
{
}

GLWindow::Context::~Context()
{
}

//------------------------------------------------------------------------

GLRenderer::GLRenderer(weak<const FileSystem> filesystem)
:   Windowing::Renderer(gameArena())
,   m_context(scoped<Context>::create(arena(), (PlatformData*)getPlatformData()))
,   m_filesystem(filesystem)
{
}

GLRenderer::~GLRenderer()
{
}

void GLRenderer::attachWindow(weak<GLWindow> w) const
{
    be_assert(Thread::currentId() == m_context->m_threadId, "render command on wrong thread");
    w->m_context = scoped<GLWindow::Context>::create(arena(), m_context->m_glContext, m_context->m_threadId);
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
    if (m_context)
    {
        be_assert(Thread::currentId() == m_context->m_threadId, "render command on wrong thread");
        ::Window* w = (::Window*)getWindowHandle();
        weak<GLRenderer::Context> c = be_checked_cast<GLRenderer>(m_renderer)->m_context;
        if(!glXMakeCurrent(c->m_display, *w, c->m_glContext))
            be_error("Unable to set current context");
    }
}

void GLWindow::clearCurrent() const
{
    if (m_context)
    {
        be_assert(Thread::currentId() == m_context->m_threadId, "render command on wrong thread");
        weak<GLRenderer::Context> c = be_checked_cast<GLRenderer>(m_renderer)->m_context;
        if(!glXMakeCurrent(c->m_display, c->m_defaultWindow, c->m_glContext))
            be_error("Unable to clear current context");
    }
}

void GLWindow::present() const
{
    if (m_context)
    {
        be_assert(Thread::currentId() == m_context->m_threadId, "render command on wrong thread");
        ::Window* w = (::Window*)getWindowHandle();
        glXSwapBuffers(be_checked_cast<GLRenderer>(m_renderer)->m_context->m_display, *w);
    }
}


}}}

