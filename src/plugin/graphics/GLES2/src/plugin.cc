/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <stdafx.h>
#include    <glesrenderer.hh>
#include    <plugin/plugin.hh>

static ref<BugEngine::OpenGLES::GLESRenderer> create(const BugEngine::Plugin::Context& context)
{
    return ref<BugEngine::OpenGLES::GLESRenderer>::create(BugEngine::Arena::game(), context);
}

BE_PLUGIN_REGISTER(BugEngine::IRenderer, &create);
