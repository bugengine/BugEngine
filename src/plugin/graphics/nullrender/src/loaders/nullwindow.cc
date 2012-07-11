/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <loaders/nullwindow.hh>
#include    <nullrenderer.hh>
#include    <3d/rendertarget/rendertarget.script.hh>


namespace BugEngine { namespace Null
{

void NullWindow::begin(ClearMode /*clear*/) const
{
}

void NullWindow::end(PresentMode /*presentMode*/) const
{
}

NullWindow::NullWindow(weak<const RenderWindow> resource, weak<const NullRenderer> renderer)
:   IRenderTarget(resource, renderer)
{
}

NullWindow::~NullWindow()
{
}

void NullWindow::present() const
{
}

void NullWindow::load(weak<const Resource> /*resource*/)
{
}

void NullWindow::unload()
{
}

}}
