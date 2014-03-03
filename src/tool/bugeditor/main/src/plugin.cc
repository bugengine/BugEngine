/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <stdafx.h>
#include    <editor.hh>
#include    <plugin/plugin.hh>
#include    <bugengine/application.hh>

static ref<BugEngine::Editor::Editor> create(const BugEngine::Plugin::Context& context)
{
    return ref<BugEngine::Editor::Editor>::create(BugEngine::Arena::game(), context);
}
BE_PLUGIN_REGISTER(BugEngine::Application, &create);
