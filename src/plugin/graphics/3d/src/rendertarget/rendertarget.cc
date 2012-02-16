/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
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
    :   dimensions(width, height)
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
