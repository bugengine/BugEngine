/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <3d/stdafx.h>
#include    <3d/rendertarget/rendertarget.script.hh>

namespace BugEngine
{

RenderTarget::RenderTarget()
{
}

RenderTarget::~RenderTarget()
{
}

RenderSurface::RenderSurface(u16 width, u16 height)
    :   dimensions(make_uint2(width, height))
{
}

RenderSurface::~RenderSurface()
{
}

RenderWindow::RenderWindow(istring title)
    :   title(title)
{
}

RenderWindow::~RenderWindow()
{
}

}
