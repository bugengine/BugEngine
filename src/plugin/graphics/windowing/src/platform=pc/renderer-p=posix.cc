/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <windowing/stdafx.h>
#include    <windowing/renderer.hh>
#include    <windowing/window.hh>
#include    <posix/platformrenderer.hh>
#include    <GL/glx.h>
#include    <core/threads/event.hh>
#include    <X11/keysym.h>
#include    <X11/Xatom.h>

namespace BugEngine { namespace Windowing
{

namespace
{
    static GLXFBConfig selectGLXFbConfig(::Display* display, int screen)
    {
        static int s_glxAttributes[] = {
            GLX_X_RENDERABLE,   True,
            GLX_DRAWABLE_TYPE,  GLX_WINDOW_BIT,
            GLX_RENDER_TYPE,    GLX_RGBA_BIT,
            //GLX_X_VISUAL_TYPE,  GLX_TRUE_COLOR,
            //GLX_RED_SIZE, 8,
            //GLX_GREEN_SIZE, 8,
            //GLX_BLUE_SIZE, 8,
            //GLX_ALPHA_SIZE, 8,
            //GLX_DEPTH_SIZE, 24,
            //GLX_STENCIL_SIZE, 8,
            GLX_DOUBLEBUFFER, True,
            None
        };

        int configCount;
        GLXFBConfig *configs = glXChooseFBConfig(display, screen, s_glxAttributes, &configCount);
        be_info("found %d configs" | configCount);
        GLXFBConfig fbConfig = configs[0];
        XFree(configs);
        return fbConfig;
    }
}

PlatformData::PlatformData(::Display* display)
:   display(display)
,   fbConfig(display ? selectGLXFbConfig(display, XDefaultScreen(display)) : 0)
,   visual(display ? glXGetVisualFromFBConfig(display, fbConfig) : 0)
{
}

Renderer::PlatformRenderer::PlatformRenderer()
:   m_platformData(XOpenDisplay(0))
,   m_windowProperty(m_platformData.display
                        ?   XInternAtom(m_platformData.display, "BE_WINDOW", False)
                        :   0)
{
    if (m_platformData.display)
    {
        XSetErrorHandler(&Renderer::PlatformRenderer::xError);
        XSync(m_platformData.display, false);
    }
}

Renderer::PlatformRenderer::~PlatformRenderer()
{
    if (m_platformData.display)
    {
        XFree(m_platformData.visual);
        XCloseDisplay(m_platformData.display);
    }
}


static const char *s_messages[] =
{
    "Success",
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

int Renderer::PlatformRenderer::xError(::Display* /*display*/, XErrorEvent* event)
{
    const u32 size = sizeof(s_messages)/sizeof(s_messages[0]);
    minitl::format<1024u> errorCode = minitl::format<1024u>("%d") | event->error_code;
    const char* message = event->error_code < size
                            ?   s_messages[event->error_code]
                            :   errorCode.c_str();
    be_error("X11 error: %d (%s)"|event->error_code | message);
    return 0;
}

::Window Renderer::PlatformRenderer::createWindow(i16 x, i16 y, u16 w, u16 h)
{
    XSetWindowAttributes attributes;
    attributes.colormap = XCreateColormap(m_platformData.display, XRootWindow(m_platformData.display, m_platformData.visual->screen), m_platformData.visual->visual, AllocNone);
    attributes.border_pixel = 0;
    attributes.override_redirect = false; //flags.fullscreen
    attributes.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask;
    int attributeMask = CWBorderPixel | CWEventMask | CWOverrideRedirect | CWColormap;
    ::Window result = XCreateWindow(m_platformData.display,
                                    XRootWindow(m_platformData.display, m_platformData.visual->screen),
                                    x, y,
                                    w, h,
                                    1,
                                    m_platformData.visual->depth,
                                    InputOutput,
                                    m_platformData.visual->visual,
                                    attributeMask,
                                    &attributes);
    if (result)
    {
        XMapRaised(m_platformData.display, result);
    }
    XSync(m_platformData.display, false);
    return result;
}



Renderer::Renderer(minitl::Allocator& arena, weak<Resource::ResourceManager> manager)
    :   IRenderer(arena, manager, Scheduler::MainThread)
    ,   m_platformRenderer(scoped<PlatformRenderer>::create(arena))
{
}

Renderer::~Renderer()
{
    flush();
}

bool Renderer::success() const
{
    return m_platformRenderer->m_platformData.display != 0;
}

uint2 Renderer::getScreenSize() const
{
    Screen* s = XScreenOfDisplay(m_platformRenderer->m_platformData.display, XDefaultScreen(m_platformRenderer->m_platformData.display));
    return make_uint2(XWidthOfScreen(s), XHeightOfScreen(s));
}

void Renderer::flush()
{
    IRenderer::flush();
    XEvent event;
    /* wait for events*/
    while (XPending(m_platformRenderer->m_platformData.display) > 0)
    {
        XNextEvent(m_platformRenderer->m_platformData.display, &event);
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
                XGetWindowProperty(m_platformRenderer->m_platformData.display, event.xkey.window, m_platformRenderer->m_windowProperty, 0, sizeof(Window*)/4,
                                   False, AnyPropertyType, &type, &format, &nbItems, &leftBytes, &result);
                be_assert(result, "could not retrieve engine window handle from X11 window");
                be_info("%d items (%d): %p" | nbItems | leftBytes | *(Window**)result);
                XFree(result);
            }
            break;
        default:
            break;
        }
    }
}

void* Renderer::getPlatformData()
{
    return &m_platformRenderer->m_platformData;
}

}}
