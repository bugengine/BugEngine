/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
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

NullWindow::NullWindow(weak<const RenderWindowDescription> windowDescription, weak<const NullRenderer> renderer)
:   IRenderTarget(windowDescription, renderer)
{
}

NullWindow::~NullWindow()
{
}

void NullWindow::present() const
{
}

void NullWindow::load(weak<const Resource::Description> /*windowDescription*/)
{
}

void NullWindow::unload()
{
}

}}
