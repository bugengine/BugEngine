/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <loaders/nullrendertarget.hh>
#include    <nullrenderer.hh>
#include    <3d/rendertarget/rendertarget.script.hh>


namespace BugEngine { namespace Graphics { namespace Null
{

void NullRenderTarget::begin(ClearMode /*clear*/) const
{
}

void NullRenderTarget::end(PresentMode /*presentMode*/) const
{
}

NullRenderTarget::NullRenderTarget(weak<const RenderTarget> resource, weak<const NullRenderer> renderer)
:   IRenderTarget(resource, renderer)
{
}

NullRenderTarget::~NullRenderTarget()
{
}

void NullRenderTarget::present() const
{
}

void NullRenderTarget::load(weak<const Resource> resource)
{
}

void NullRenderTarget::unload()
{
}

}}}

