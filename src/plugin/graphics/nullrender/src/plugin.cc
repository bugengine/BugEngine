/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <stdafx.h>
#include    <nullrenderer.hh>
#include    <plugin/plugin.hh>

static ref<BugEngine::Null::NullRenderer> create(const BugEngine::Plugin::Context& context)
{
    return ref<BugEngine::Null::NullRenderer>::create(BugEngine::Arena::game(), context);
}

BE_PLUGIN_REGISTER(BugEngine::IRenderer, &create);
