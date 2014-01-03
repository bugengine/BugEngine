/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
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

NullSurface::NullSurface(weak<const RenderSurfaceDescription> surfaceDescription, weak<const NullRenderer> renderer)
:   IRenderTarget(surfaceDescription, renderer)
{
}

NullSurface::~NullSurface()
{
}

void NullSurface::present() const
{
}

void NullSurface::load(weak<const Resource::Description> /*surfaceDescription*/)
{
}

void NullSurface::unload()
{
}

}}
