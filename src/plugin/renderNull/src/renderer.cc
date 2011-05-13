/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <renderer.hh>

#include    <window.hh>

namespace BugEngine { namespace Graphics { namespace NullRenderer
{

ref<IRenderTarget> Renderer::createRenderWindow(WindowFlags flags)
{
    return ref<Window>::create(gameArena(), this, flags);
}

ref<IRenderTarget> Renderer::createRenderBuffer(TextureFlags /*flags*/)
{
    return ref<Window>();
}

ref<IRenderTarget> Renderer::createMultipleRenderBuffer(TextureFlags /*flags*/, size_t /*count*/)
{
    return ref<Window>();
}

void Renderer::flush()
{
}

Renderer::Renderer(weak<const FileSystem> filesystem)
:   m_filesystem(filesystem)
{
}

Renderer::~Renderer()
{
}

}}}

