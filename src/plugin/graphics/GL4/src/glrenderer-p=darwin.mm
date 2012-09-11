/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <GL4/stdafx.h>
#include    <GL4/glrenderer.hh>
#include    <GL4/glmemoryprovider.hh>
#include    <extensions.hh>

#include    <3d/mesh/mesh.script.hh>
#include    <3d/texture/texture.script.hh>
#include    <3d/shader/shader.script.hh>
#include    <3d/rendertarget/rendertarget.script.hh>
#include    <loaders/mesh/glmesh.hh>
#include    <loaders/texture/gltexture.hh>
#include    <loaders/shader/glshader.hh>
#include    <loaders/rendertarget/glwindow.hh>
#include    <core/threads/thread.hh>


#include    <Cocoa/Cocoa.h>
#include    <OpenGL/OpenGL.h>

@interface BugEngineOpenGLView : NSView
{
@public
    NSOpenGLContext*                                m_context;
}

- (id) initWithFrame:(NSRect) frame context:(NSOpenGLContext*) context;


@end

@implementation BugEngineOpenGLView

- (id) initWithFrame:(NSRect) frame context:(NSOpenGLContext*) context
{
    [super initWithFrame: frame];
    m_context = context;
    [m_context retain];
    GLint sync = 0;
    [m_context setValues:&sync  forParameter:NSOpenGLCPSwapInterval];
    return self;
}


- (void) dealloc
{
    be_info("destroying OpenGL view");
    [m_context release];
    [super dealloc];
}

@end

namespace BugEngine { namespace OpenGL
{

class GLRenderer::Context : public minitl::refcountable
{
    friend class GLRenderer;
    friend class GLWindow;
private:
    NSOpenGLPixelFormat*    m_pixelFormat;
    NSOpenGLContext*        m_context;
    ShaderExtensions        m_shaderext;
public:
    Context();
    ~Context();
};

static NSOpenGLPixelFormatAttribute s_attributes[] = {
    //NSOpenGLPFAAllRenderers,
    NSOpenGLPFADoubleBuffer,
    NSOpenGLPFAWindow,
    NSOpenGLPFAAccelerated,
    NSOpenGLPFANoRecovery,
    NSOpenGLPFAMinimumPolicy,
    NSOpenGLPFAColorSize, (NSOpenGLPixelFormatAttribute)24,
    NSOpenGLPFAAlphaSize, (NSOpenGLPixelFormatAttribute)8,
    NSOpenGLPFADepthSize, (NSOpenGLPixelFormatAttribute)24,
    (NSOpenGLPixelFormatAttribute)0
};

GLRenderer::Context::Context()
    :   m_pixelFormat([[NSOpenGLPixelFormat alloc] initWithAttributes: s_attributes])
    ,   m_context([[NSOpenGLContext alloc] initWithFormat: m_pixelFormat shareContext: nil])
{
    GLint sync = 0;
    [m_context setValues:&sync forParameter:NSOpenGLCPSwapInterval];
    [m_context makeCurrentContext];
    be_info("Created OpenGL context %s (%s) on %s"
        | (const char*)glGetString(GL_VERSION)
        | (const char *)glGetString(GL_VENDOR)
        | (const char*)glGetString(GL_RENDERER));
}

GLRenderer::Context::~Context()
{
    [m_context release];
    [m_pixelFormat release];
}


class GLWindow::Context : public minitl::refcountable
{
    friend class GLRenderer;
    friend class GLWindow;
private:
    NSWindow*               m_window;
    BugEngineOpenGLView*    m_view;
    CGLContextObj           m_context;
    u64                     m_threadId;
public:
    Context(NSWindow* window, NSOpenGLContext* context, u64 threadId);
    ~Context();
};

GLWindow::Context::Context(NSWindow* window, NSOpenGLContext* context, u64 threadId)
    :   m_window(window)
    ,   m_view([[BugEngineOpenGLView alloc] initWithFrame:[m_window contentRectForFrameRect:[m_window frame]]
                                                  context: context])
    ,   m_context((CGLContextObj)[context CGLContextObj])
    ,   m_threadId(threadId)
{
    [window setContentView:m_view];
    [window makeKeyAndOrderFront: nil];
    [m_view->m_context setView: m_view];
}

GLWindow::Context::~Context()
{
    [m_window setContentView:nil];
    [m_view release];
}

//------------------------------------------------------------------------

GLRenderer::GLRenderer(const Plugin::Context& context)
    :   Windowing::Renderer(Arena::general(), context.resourceManager)
    ,   m_context(scoped<Context>::create(Arena::general()))
    ,   m_openGLMemoryProvider(scoped<GLMemoryProvider>::create(Arena::general()))
    ,   m_openCLScheduler("kernel.opencl_gl", context)
{
    [NSOpenGLContext clearCurrentContext];
}

GLRenderer::~GLRenderer()
{
}

void GLRenderer::attachWindow(weak<GLWindow> w) const
{
    NSWindow* window = (NSWindow*)w->getWindowHandle();
    NSOpenGLContext* context = [[NSOpenGLContext alloc] initWithFormat: m_context->m_pixelFormat
                                                          shareContext: m_context->m_context];
    be_assert(window, "No native window created for BugEngine window");
    w->m_context.reset(scoped<GLWindow::Context>::create(Arena::general(), window, context, Thread::currentId()));
    [context release];
}

const ShaderExtensions& GLRenderer::shaderext() const
{
    be_assert(m_context, "extensions required before context was created");
    return m_context->m_shaderext;
}

//------------------------------------------------------------------------

GLWindow::GLWindow(weak<const RenderWindowDescription> windowDescription, weak<const GLRenderer> renderer)
:   Windowing::Window(windowDescription, renderer)
,   m_context(scoped<Context>())
{
}

GLWindow::~GLWindow()
{
}

void GLWindow::load(weak<const Resource::Description> windowDescription)
{
    Window::load(windowDescription);
    be_checked_cast<const GLRenderer>(m_renderer)->attachWindow(this);
}

void GLWindow::unload()
{
    be_assert(Thread::currentId() == m_context->m_threadId, "render command on wrong thread");
    Window::unload();
    m_context.reset(scoped<Context>());
}

void GLWindow::setCurrent() const
{
    if(m_context)
    {
        be_assert(Thread::currentId() == m_context->m_threadId, "render command on wrong thread: Window belongs to thread %d, current thread: %d" | m_context->m_threadId | Thread::currentId());
        [m_context->m_view->m_context makeCurrentContext];
    }
}

void GLWindow::clearCurrent() const
{
    if(m_context)
    {
        be_assert(Thread::currentId() == m_context->m_threadId, "render command on wrong thread: Window belongs to thread %d, current thread: %d" | m_context->m_threadId | Thread::currentId());
        [NSOpenGLContext clearCurrentContext];
    }
}

void GLWindow::present() const
{
    if(m_context)
    {
        be_assert(Thread::currentId() == m_context->m_threadId, "render command on wrong thread: Window belongs to thread %d, current thread: %d" | m_context->m_threadId | Thread::currentId());
        CGLFlushDrawable(m_context->m_context);
    }
}


}}
