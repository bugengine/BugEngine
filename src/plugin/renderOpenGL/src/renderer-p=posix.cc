/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <renderer.hh>
#include    <extensions.hh>
#include    <window.hh>
#include    <loaders/mesh/meshloader.script.hh>
#include    <loaders/texture/textureloader.script.hh>
#include    <loaders/shader/shaderloader.script.hh>

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
    const ShaderExtensions  shaderext;
public:
    Context(::Display* display, ::GLXFBConfig fbConfig);
    ~Context();
};

static GLXContext createContext(::Display* display, ::GLXFBConfig fbConfig)
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

Renderer::Context::Context(::Display* display, ::GLXFBConfig fbConfig)
:   m_display(display)
,   m_glContext(createContext(display, fbConfig))
,   shaderext()
{
    XSync(m_display, false);
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
,   m_meshLoader(scoped<const MeshLoader>::create(gameArena()))
,   m_textureLoader(scoped<const TextureLoader>::create(gameArena()))
,   m_shaderLoader(scoped<const ShaderLoader>::create(gameArena(), this))
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
        ::Window* handle = (::Window*)(w->getWindowHandle());
        glXMakeCurrent(m_context->m_display, *handle, m_context->m_glContext);
        be_info("Creating OpenGL %s (%s)" | (const char*)glGetString(GL_VERSION) | (const char *)glGetString(GL_VENDOR));
        glXMakeCurrent(m_context->m_display, *handle, 0);
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

const ShaderExtensions& Renderer::shaderext() const
{
    be_assert(m_context, "extensions required before context was created");
    return m_context->shaderext;
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

