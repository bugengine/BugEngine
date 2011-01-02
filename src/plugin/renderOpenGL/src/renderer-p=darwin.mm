/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
 see LICENSE for detail */

#include    <stdafx.h>
#include    <renderer.hh>
#include    <window.hh>

#include    <Cocoa/Cocoa.h>
#include    <OpenGL/OpenGL.h>

@interface BugEngineOpenGLView : NSView
{
@public
    NSOpenGLContext*        m_context;
}

- (id) initWithFrame:(NSRect) frame context:(NSOpenGLContext*) context;


@end

@implementation BugEngineOpenGLView

- (id) initWithFrame:(NSRect) frame context:(NSOpenGLContext*) context
{
    [super initWithFrame: frame];
    m_context = context;
    [m_context retain];
    int sync = 0;
    [m_context setValues:&sync forParameter:NSOpenGLCPSwapInterval];
    return self;
}


- (void) dealloc
{
    be_info("destroying OpenGL view");
    [m_context release];
    [super dealloc];
}

@end

namespace BugEngine { namespace Graphics { namespace OpenGL
{

class Renderer::Context : public minitl::refcountable
{
    friend class Renderer;
    friend class Window;
private:
    NSOpenGLPixelFormat*    m_pixelFormat;
    NSOpenGLContext*        m_context;
public:
    Context();
    ~Context();
};

static const NSOpenGLPixelFormatAttribute s_attributes[] = {
    //NSOpenGLPFAAllRenderers,
    NSOpenGLPFADoubleBuffer,
    NSOpenGLPFAWindow,
    NSOpenGLPFAAccelerated,
    NSOpenGLPFANoRecovery,
    NSOpenGLPFAMinimumPolicy,
    NSOpenGLPFAColorSize, (CGLPixelFormatAttribute)24,
    NSOpenGLPFAAlphaSize, (CGLPixelFormatAttribute)8,
    NSOpenGLPFADepthSize, (CGLPixelFormatAttribute)24,
    (NSOpenGLPixelFormatAttribute)0
};

Renderer::Context::Context()
    :   m_pixelFormat([[NSOpenGLPixelFormat alloc] initWithAttributes: s_attributes])
    ,   m_context([[NSOpenGLContext alloc] initWithFormat: m_pixelFormat shareContext: nil])
{
    int sync = 0;
    [m_context setValues:&sync forParameter:NSOpenGLCPSwapInterval];
}

Renderer::Context::~Context()
{
    [m_context release];
    [m_pixelFormat release];
}


class Window::Context : public minitl::refcountable
{
    friend class Renderer;
    friend class Window;
private:
    NSWindow*               m_window;
    BugEngineOpenGLView*    m_view;
    CGLContextObj           m_context;
public:
    Context(NSWindow* window, NSOpenGLContext* context);
    ~Context();
};

Window::Context::Context(NSWindow* window, NSOpenGLContext* context)
    :   m_window(window)
    ,   m_view([[BugEngineOpenGLView alloc] initWithFrame:[m_window contentRectForFrameRect:[m_window frame]]
                                                  context: context])
    ,   m_context((CGLContextObj)[context CGLContextObj])
{
    [window setContentView:m_view];
    [window makeKeyAndOrderFront: nil];
    [m_view->m_context setView: m_view];
}

Window::Context::~Context()
{
    [m_window setContentView:nil];
    [m_view release];
}

//------------------------------------------------------------------------

Renderer::Renderer(weak<const FileSystem> filesystem)
:   m_context(scoped<Renderer::Context>::create(gameArena()))
,   m_filesystem(filesystem)
{
}

Renderer::~Renderer()
{
    destroyContextAsync();
}

void Renderer::attachWindow(Window* w)
{
    NSWindow* window = (NSWindow*)w->getWindowHandle();
    NSOpenGLContext* context = [[NSOpenGLContext alloc] initWithFormat: m_context->m_pixelFormat
                                                          shareContext: m_context->m_context];
    be_assert(window, "No native window created for BugEngine window");
    w->m_context = scoped<Window::Context>::create(gameArena(), window, context);
    [context release];
}

void Renderer::createContext(void* params)
{
}

void Renderer::destroyContext()
{
    m_context = scoped<Renderer::Context>();
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
    if(!isClosed())
    {
        [m_context->m_view->m_context makeCurrentContext];
    }
}

void Window::clearCurrent()
{
    if(!isClosed())
    {
        [NSOpenGLContext clearCurrentContext];
    }
}

void Window::present()
{
    if(!isClosed())
    {
        CGLFlushDrawable(m_context->m_context);
    }
}


}}}

