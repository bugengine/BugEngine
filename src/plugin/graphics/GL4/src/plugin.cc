/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <GL4/stdafx.h>
#include    <GL4/glrenderer.hh>
#include    <plugin/plugin.hh>

static ref<BugEngine::OpenGL::GLRenderer> create(const BugEngine::Plugin::Context& context)
{
    return ref<BugEngine::OpenGL::GLRenderer>::create(BugEngine::Arena::game(), context);
}

BE_PLUGIN_REGISTER(BugEngine::IRenderer, &create);
