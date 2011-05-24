/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <windowing/stdafx.h>
#include    <windowing/renderer.hh>
#include    <windowing/window.hh>
#include    <posix/platformrenderer.hh>
#include    <GL/glx.h>
#include    <core/threads/event.hh>
#include    <X11/keysym.h>
#include    <X11/Xatom.h>

namespace BugEngine { namespace Graphics { namespace Windowing
{

namespace
{
    static GLXFBConfig selectGLXFbConfig(::Display* display, int screen)
    {
        static int s_glxAttributes[] = {
            GLX_X_RENDERABLE,   True,
            GLX_DRAWABLE_TYPE,  GLX_WINDOW_BIT,
            GLX_RENDER_TYPE,    GLX_RGBA_BIT,
            GLX_X_VISUAL_TYPE,  GLX_TRUE_COLOR,
            GLX_RED_SIZE, 8,
            GLX_GREEN_SIZE, 8,
            GLX_BLUE_SIZE, 8,
            GLX_ALPHA_SIZE, 8,
            GLX_DEPTH_SIZE, 24,
            GLX_STENCIL_SIZE, 8,
            GLX_DOUBLEBUFFER, True,
            None
        };

        int configCount;
        GLXFBConfig *configs = glXChooseFBConfig(display, screen, s_glxAttributes, &configCount);
        GLXFBConfig fbConfig = configs[0];
        XFree(configs);
        return fbConfig;
    }
}

Renderer::PlatformRenderer::PlatformRenderer()
:   m_display(XOpenDisplay(0))
,   m_screen(XDefaultScreen(m_display))
,   m_rootWindow(XRootWindow(m_display, m_screen))
,   m_fbConfig(selectGLXFbConfig(m_display, m_screen))
,   m_visual(glXGetVisualFromFBConfig(m_display, m_fbConfig))
,   m_windowProperty(XInternAtom(m_display, "BE_WINDOW", False))
{
    XSetErrorHandler(&Renderer::PlatformRenderer::xError);
    XSync(m_display, false);
}

Renderer::PlatformRenderer::~PlatformRenderer()
{
    XFree(m_visual);
    if (m_display)
    {
        XCloseDisplay(m_display);
    }
}

static const char *s_messages[] =
{
    "Success"
    "BadRequest",
    "BadValue",
    "BadWindow",
    "BadPixmap",
    "BadAtom",
    "BadCursor",
    "BadFont",
    "BadMatch",
    "BadDrawable",
    "BadAccess",
    "BadAlloc",
    "BadColor",
    "BadGC",
    "BadIDChoice",
    "BadName",
    "BadLength",
    "BadImplementation"
};

int Renderer::PlatformRenderer::xError(::Display* display, XErrorEvent* event)
{
    be_assert(false, "X11 error: %s"|s_messages[event->error_code]);
    return 0;
}

::Window Renderer::PlatformRenderer::createWindow(const WindowFlags& flags)
{
    XSetWindowAttributes attributes;
    attributes.colormap = XCreateColormap(m_display, XRootWindow(m_display, m_visual->screen), m_visual->visual, AllocNone);
    attributes.border_pixel = 0;
    attributes.override_redirect = flags.fullscreen;
    attributes.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask;
    int attributeMask = CWBorderPixel | CWEventMask | CWOverrideRedirect | CWColormap;
    ::Window result = XCreateWindow(m_display,
                                    XRootWindow(m_display, m_visual->screen),
                                    flags.position.x(), flags.position.y(),
                                    flags.size.x(), flags.size.y(),
                                    1,
                                    m_visual->depth,
                                    InputOutput,
                                    m_visual->visual,
                                    attributeMask,
                                    &attributes);
    if (result)
    {
        XMapRaised(m_display, result);
    }
    XSync(m_display, false);
    return result;
}



Renderer::Renderer(Allocator& arena)
    :   IRenderer(arena)
    ,   m_platformRenderer(scoped<PlatformRenderer>::create(arena))
{
}

Renderer::~Renderer()
{
    flush();
}

uint2 Renderer::getScreenSize()
{
    Screen* s = XScreenOfDisplay(m_platformRenderer->m_display, m_platformRenderer->m_screen);
    return uint2(XWidthOfScreen(s), XHeightOfScreen(s));
}

void Renderer::flush()
{
    XEvent event;
    /* wait for events*/ 
    while (XPending(m_platformRenderer->m_display) > 0)
    {
        XNextEvent(m_platformRenderer->m_display, &event);
        switch (event.type)
        {
        case DestroyNotify:
            be_info("destroy");
            break;
        case Expose:
            if (event.xexpose.count != 0)
                break;
            be_info("exposure");
            break;
        case ConfigureNotify:
            be_info("configure");
            break;
        case ButtonPress:
            break;
        case KeyPress:
            if (XLookupKeysym(&event.xkey, 0) == XK_Escape)
            {
                ::Atom type;
                int format;
                unsigned long nbItems;
                unsigned long leftBytes;
                unsigned char *result = 0;
                XGetWindowProperty(m_platformRenderer->m_display, event.xkey.window, m_platformRenderer->m_windowProperty, 0, sizeof(Window*)/4,
                                   False, AnyPropertyType, &type, &format, &nbItems, &leftBytes, &result);
                be_assert(result, "could not retrieve engine window handle from X11 window");
                Window *w = *(Window**)result;
                be_info("%d items (%d): %p" | nbItems | leftBytes | w);
                w->close();
                XFree(result);
            }
            break;
        default:
            break;
        }
    }
}

}}}
