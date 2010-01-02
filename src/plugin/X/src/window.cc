/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <X/stdafx.h>
#include    <X/window.hh>
#include    <X/renderer.hh>

namespace BugEngine { namespace Graphics { namespace X
{

Window::Attributes::Attributes(::Display* display, ::Window root, ::Visual* visual, WindowFlags flags)
{
    m_attributes.border_pixel = 0;
    m_attributes.override_redirect = !flags.fullscreen;
    m_attributes.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask;
    m_attributeMask = CWBorderPixel | CWEventMask | CWOverrideRedirect;
}

Window::Attributes::~Attributes()
{
}

Window::Window(weak<Renderer> renderer, WindowFlags flags, ref<const Scene> scene)
:   RenderTarget(scene)
,   m_renderer(renderer)
,   m_attributes(renderer->m_display, renderer->m_rootWindow, renderer->m_visual->visual, flags)
,   m_window(XCreateWindow(renderer->m_display,
                           renderer->m_rootWindow,
                           0, 0,
                           flags.size.x(), flags.size.y(),
                           0,
                           CopyFromParent, InputOutput,
                           CopyFromParent, //renderer->m_visual->visual,
                           m_attributes.m_attributeMask,
                           &m_attributes.m_attributes))
{
    XMapRaised(m_renderer->m_display, m_window);
    XFlush(m_renderer->m_display);
}

Window::~Window()
{
    close();
}

void Window::close()
{
    if(m_window)
    {
        ::Window bu = m_window;
        m_window = 0;
        XDestroyWindow(m_renderer->m_display, bu);
    }
}

uint2 Window::getDimensions() const
{
    return int2(1920, 1200);
}

}}}
