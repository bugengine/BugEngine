/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <loaders/nullsurface.hh>
#include    <nullrenderer.hh>
#include    <3d/rendertarget/rendertarget.script.hh>


namespace BugEngine { namespace Null
{

void NullSurface::begin(ClearMode /*clear*/) const
{
}

void NullSurface::end(PresentMode /*presentMode*/) const
{
}

NullSurface::NullSurface(weak<const RenderSurface> resource, weak<const NullRenderer> renderer)
:   IRenderTarget(resource, renderer)
{
}

NullSurface::~NullSurface()
{
}

void NullSurface::present() const
{
}

void NullSurface::load(weak<const Resource> /*resource*/)
{
}

void NullSurface::unload()
{
}

}}
