/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
 see LICENSE for detail */

#include    <windowing/stdafx.h>
#include    <windowing/renderer.hh>
#include    <windowing/window.hh>
#include    <core/threads/event.hh>

namespace BugEngine { namespace Graphics { namespace Windowing
{

class Renderer::PlatformRenderer : public minitl::refcountable
{
};

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

uint2 Renderer::getScreenSize()
{
    return uint2(1920, 1200);
}

void Renderer::flush()
{
}

void Renderer::createContextAsync(void*)
{
}

void Renderer::destroyContextAsync(void* params)
{
}

void Renderer::createContext(void*)
{
}

void Renderer::destroyContext(void*)
{
}

}}}
