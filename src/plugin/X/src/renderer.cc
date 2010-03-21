/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <X/stdafx.h>
#include    <X/renderer.hh>
#include    <X/window.hh>
#include    <GL/glx.h>
#include    <core/threads/event.hh>

namespace BugEngine { namespace Graphics { namespace X
{

namespace
{
    static int s_glxAttributes[] = {
        GLX_RGBA, GLX_DOUBLEBUFFER,
        GLX_RED_SIZE, 6,
        GLX_GREEN_SIZE, 6,
        GLX_BLUE_SIZE, 6, 
        GLX_DEPTH_SIZE, 24,
        GLX_X_VISUAL_TYPE_EXT, GLX_TRUE_COLOR_EXT,
        None
    };
}

Renderer::Renderer()
:   m_display(XOpenDisplay(0))
,   m_screen(XDefaultScreen(m_display))
,   m_rootWindow(XRootWindow(m_display, m_screen))
,   m_visual(glXChooseVisual(m_display, m_screen, s_glxAttributes))
{
    //XSetErrorHandler(&Renderer::xError);
    XFlush(m_display);
}

Renderer::~Renderer()
{
    if(m_display)
        XCloseDisplay(m_display);
}

uint2 Renderer::getScreenSize()
{
    Screen* s = XScreenOfDisplay(m_display, m_screen);
    return uint2(XWidthOfScreen(s), XHeightOfScreen(s));
}

::Window Renderer::createWindow(const WindowFlags& flags)
{
    XSetWindowAttributes attributes;
    attributes.border_pixel = 0;
    attributes.override_redirect = !flags.fullscreen;
    attributes.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask;
    int attributeMask = CWBorderPixel | CWEventMask | CWOverrideRedirect;
    ::Window result = XCreateWindow(m_display,
                                    XRootWindow(m_display, m_visual->screen),
                                    0, 0,
                                    flags.size.x(), flags.size.y(),
                                    1,
                                    m_visual->depth,
                                    InputOutput,
                                    m_visual->visual,
                                    attributeMask,
                                    &attributes);
    if(result)
    {
        XMapRaised(m_display, result);
    }
    XFlush(m_display);
    return result;
}

int Renderer::step() const
{
    XEvent event;
    /* wait for events*/ 
    int exit = 0;
    while(XPending(m_display))
    {
        XNextEvent(m_display, &event);
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
            break;
        case ButtonPress:
            exit = 1;
            break;
        case KeyPress:
            if (XLookupKeysym(&event.xkey, 0) == XK_Escape)
            {
                exit = 1;
            }
            break;
        default:
            break;
        }
    }

    return exit;
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

int Renderer::xError(::Display* display, XErrorEvent* event)
{
    be_assert(false, "X11 error: %s"|s_messages[event->error_code]);
    return 0;
}


}}}
