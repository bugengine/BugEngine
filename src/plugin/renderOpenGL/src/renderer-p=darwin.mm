/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
 see LICENSE for detail */

#include    <stdafx.h>
#include    <renderer.hh>
#include    <extensions.hh>
#include    <window.hh>

#include    <graphics/renderer/igpuloader.hh>
#include    <graphics/objects/mesh.script.hh>
#include    <graphics/objects/texture.script.hh>
#include    <graphics/objects/shader.script.hh>
#include    <loaders/mesh/glmesh.hh>
#include    <loaders/texture/gltexture.hh>
#include    <loaders/shader/glshader.hh>


#include    <Cocoa/Cocoa.h>
#include    <OpenGL/OpenGL.h>

@interface BugEngineOpenGLView : NSView
{
@public
    NSOpenGLContext*        m_context;
    ShaderExtensions        shaderext;
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
    friend class GLWindow;
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


class GLWindow::Context : public minitl::refcountable
{
    friend class Renderer;
    friend class GLWindow;
private:
    NSWindow*               m_window;
    BugEngineOpenGLView*    m_view;
    CGLContextObj           m_context;
public:
    Context(NSWindow* window, NSOpenGLContext* context);
    ~Context();
};

GLWindow::Context::Context(NSWindow* window, NSOpenGLContext* context)
    :   m_window(window)
    ,   m_view([[BugEngineOpenGLView alloc] initWithFrame:[m_window contentRectForFrameRect:[m_window frame]]
                                                  context: context])
    ,   m_context((CGLContextObj)[context CGLContextObj])
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

Renderer::Renderer(weak<const FileSystem> filesystem)
:   Windowing::Renderer(gameArena())
,   m_context(scoped<Renderer::Context>::create(arena()))
,   m_filesystem(filesystem)
,   m_meshLoader(scoped<const MeshLoader>::create(arena()))
,   m_textureLoader(scoped<const TextureLoader>::create(arena()))
,   m_shaderLoader(scoped<const ShaderLoader>::create(arena(), this))
{
}

Renderer::~Renderer()
{
    destroyContext();
}

void Renderer::attachWindow(GLWindow* w)
{
    NSWindow* window = (NSWindow*)w->getWindowHandle();
    NSOpenGLContext* context = [[NSOpenGLContext alloc] initWithFormat: m_context->m_pixelFormat
                                                          shareContext: m_context->m_context];
    be_assert(window, "No native window created for BugEngine window");
    w->m_context = scoped<GLWindow::Context>::create(arena(), window, context);
    [context release];
}

const ShaderExtensions& Renderer::shaderext() const
{
    be_assert(m_context, "extensions required before context was created");
    return m_context->shaderext;
}

//------------------------------------------------------------------------

GLWindow::GLWindow(weak<Renderer> renderer, WindowFlags flags)
:   Windowing::Window(renderer, flags)
{
    renderer->attachWindow(this);
}

GLWindow::~GLWindow()
{
}

void GLWindow::setCurrent()
{
    if(!closed())
    {
        [m_context->m_view->m_context makeCurrentContext];
    }
}

void GLWindow::clearCurrent()
{
    if(!closed())
    {
        [NSOpenGLContext clearCurrentContext];
    }
}

void GLWindow::present()
{
    if(!closed())
    {
        CGLFlushDrawable(m_context->m_context);
    }
}


}}}

