/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <X/stdafx.h>
#include    <X/renderer.hh>
#include    <X/window.hh>
#include    <GL/glx.h>
#include    <core/threads/event.hh>
#include 	<X11/keysym.h>

namespace BugEngine { namespace Graphics { namespace X
{

namespace
{
    static int s_glxAttributes[] = {
        GLX_RGBA, GLX_DOUBLEBUFFER,
        None
    };
}

Renderer::Renderer()
:   m_display((XInitThreads(), XOpenDisplay(0)))
,   m_screen(XDefaultScreen(m_display))
,   m_rootWindow(XRootWindow(m_display, m_screen))
,   m_visual(glXChooseVisual(m_display, m_screen, s_glxAttributes))
,   m_windowManagementThread("X11", &windowProc, (intptr_t)this, 0)
{
    XSetErrorHandler(&Renderer::xError);
    XSync(m_display, false);
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
    if(result)
    {
        XMapRaised(m_display, result);
    }
    XSync(m_display, false);
    return result;
}

intptr_t Renderer::windowProc(intptr_t p1, intptr_t /*p2*/)
{
    Renderer* r = reinterpret_cast<Renderer*>(p1);
    XEvent event;
    /* wait for events*/ 
    int exit = 0;
    while(!exit)
    {
        XNextEvent(r->m_display, &event);
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

    return 0;
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
