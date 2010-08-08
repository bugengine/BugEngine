/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <rendertargets/multiple.hh>
#include    <renderer.hh>


namespace BugEngine { namespace Graphics { namespace DirectX9
{

MultipleRenderTarget::MultipleRenderTarget(weak<Renderer> renderer, TextureFlags /*flags*/)
:   IRenderTarget(renderer)
{
}

MultipleRenderTarget::~MultipleRenderTarget()
{
}

bool MultipleRenderTarget::closed() const
{
    return false;
}

void MultipleRenderTarget::setCurrent()
{
}

void MultipleRenderTarget::close()
{
}

void MultipleRenderTarget::begin(ClearMode /*clear*/)
{
}

void MultipleRenderTarget::end(PresentMode /*present*/)
{
}

}}}
