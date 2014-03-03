/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <stdafx.h>
#include    <textsample.hh>
#include    <plugin/plugin.hh>
#include    <bugengine/application.hh>

static ref<BugEngine::TextSample> create(const BugEngine::Plugin::Context& context)
{
    return ref<BugEngine::TextSample>::create(BugEngine::Arena::game(), context);
}

BE_PLUGIN_REGISTER(BugEngine::Application, create);
