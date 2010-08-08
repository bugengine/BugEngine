/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <rendertargets/offscreen.hh>
#include    <renderer.hh>


namespace BugEngine { namespace Graphics { namespace DirectX9
{

OffscreenRenderTarget::OffscreenRenderTarget(weak<Renderer> renderer, TextureFlags /*flags*/)
:   IRenderTarget(renderer)
{
}

OffscreenRenderTarget::~OffscreenRenderTarget()
{
}

bool OffscreenRenderTarget::closed() const
{
    return false;
}

void OffscreenRenderTarget::setCurrent()
{
}

void OffscreenRenderTarget::close()
{
}

void OffscreenRenderTarget::begin(ClearMode /*clear*/)
{
}

void OffscreenRenderTarget::end(PresentMode /*present*/)
{
}

}}}
