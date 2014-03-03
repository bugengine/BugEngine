/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <stdafx.h>
#include    <dx9renderer.hh>
#include    <plugin/plugin.hh>

static ref<BugEngine::DirectX9::Dx9Renderer> create(const BugEngine::Plugin::Context& context)
{
    return ref<BugEngine::DirectX9::Dx9Renderer>::create(BugEngine::Arena::game(), context);
}

BE_PLUGIN_REGISTER(BugEngine::IRenderer, &create);
