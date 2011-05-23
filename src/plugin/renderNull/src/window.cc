/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <window.hh>
#include    <renderer.hh>


namespace BugEngine { namespace Graphics { namespace NullRenderer
{

void Window::begin(ClearMode clear)
{
}

void Window::end(PresentMode presentMode)
{
}

Window::Window(weak<Renderer> renderer)
:   IRenderTarget(renderer)
,   m_closed(false)
{
}

Window::~Window()
{
}

void Window::close()
{
    m_closed = true;
}

bool Window::closed() const
{
    return m_closed;
}

void Window::present()
{
}


}}}

