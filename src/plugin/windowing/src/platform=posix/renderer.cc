/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <X/stdafx.h>
#include    <X/renderer.hh>
#include    <X/window.hh>
#include    <GL/glx.h>
#include    <core/threads/event.hh>
#include    <X11/keysym.h>
#include    <X11/Xatom.h>

namespace BugEngine { namespace Graphics { namespace X
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

Renderer::Renderer()
:   m_display(XOpenDisplay(0))
,   m_screen(XDefaultScreen(m_display))
,   m_rootWindow(XRootWindow(m_display, m_screen))
,   m_fbConfig(selectGLXFbConfig(m_display, m_screen))
,   m_visual(glXGetVisualFromFBConfig(m_display, m_fbConfig))
,   m_windowProperty(XInternAtom(m_display, "BE_WINDOW", False))
{
    XSetErrorHandler(&Renderer::xError);
    XSync(m_display, false);
}

Renderer::~Renderer()
{
    XFree(m_visual);
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

void Renderer::flush()
{
    XEvent event;
    /* wait for events*/ 
    while(XPending(m_display) > 0)
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
            be_info("configure");
            break;
        case ButtonPress:
            break;
        case KeyPress:
            if (XLookupKeysym(&event.xkey, 0) == XK_Escape)
            {
                Window* w = 0;
                ::Atom type;
                int format;
                unsigned long nbItems;
                unsigned long leftBytes;
                XGetWindowProperty(m_display, event.xkey.window, m_windowProperty, 0, sizeof(Window*)/4,
                                   False, XA_INTEGER, &type, &format, &nbItems, &leftBytes, (unsigned char**)&w);
                be_assert(w, "could not retrieve engine window handle from X11 window");
                be_info("%d items: %p" | nbItems | (const void*)w);
                w->close();
                XFree(w);
            }
            break;
        default:
            break;
        }
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

int Renderer::xError(::Display* display, XErrorEvent* event)
{
    be_assert(false, "X11 error: %s"|s_messages[event->error_code]);
    return 0;
}


}}}
