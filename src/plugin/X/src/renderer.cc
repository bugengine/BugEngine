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
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8, 
        GLX_DEPTH_SIZE, 24,
        None
    };
}

Renderer::Renderer()
:   m_display(XOpenDisplay(0))
,   m_screen(XDefaultScreen(m_display))
,   m_rootWindow(XRootWindow(m_display, m_screen))
,   m_visual(glXChooseVisual(m_display, m_screen, s_glxAttributes))
,   m_exit(false)
,   m_windowManagementThread("WindowManagement", &Renderer::updateWindows, (intptr_t)this, 0, Thread::AboveNormal)   
{
    XSetErrorHandler(&Renderer::xError);
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

int Renderer::step() const
{
    return (int)m_exit;
}

intptr_t Renderer::updateWindows(intptr_t p1, intptr_t p2)
{
    weak<Renderer> renderer = (Renderer*)p1;
    XEvent event;
    /* wait for events*/ 
    while (!renderer->m_exit)
    {
        XNextEvent(renderer->m_display, &event);
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
            renderer->m_exit = true;
            break;
        case KeyPress:
            if (XLookupKeysym(&event.xkey, 0) == XK_Escape)
            {
                renderer->m_exit = true;
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
