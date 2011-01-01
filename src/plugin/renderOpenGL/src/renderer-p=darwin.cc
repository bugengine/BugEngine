/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
 see LICENSE for detail */

#include    <stdafx.h>
#include    <renderer.hh>
#include    <window.hh>

namespace BugEngine { namespace Graphics { namespace OpenGL
{
        
class Renderer::Context : public minitl::refcountable
{
    friend class Renderer;
    friend class Window;
private:
public:
    Context();
    ~Context();
};

Renderer::Context::Context()
{
}

Renderer::Context::~Context()
{
}


class Window::Context : public minitl::refcountable
{
    friend class Renderer;
    friend class Window;
private:
public:
    Context();
    ~Context();
};

Window::Context::Context()
{
}

Window::Context::~Context()
{
}

//------------------------------------------------------------------------

Renderer::Renderer(weak<const FileSystem> filesystem)
:   m_context()
,   m_filesystem(filesystem)
{
}

Renderer::~Renderer()
{
    destroyContextAsync(0);
}

void Renderer::attachWindow(Window* w)
{
}

void Renderer::createContext(void* params)
{
}

void Renderer::destroyContext(void*)
{
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
    }
}

void Window::clearCurrent()
{
    if(!isClosed())
    {
    }
}

void Window::present()
{
    if(!isClosed())
    {
    }
}


}}}

