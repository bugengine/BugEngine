/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <GL4/stdafx.h>
#include    <GL4/glrenderer.hh>
#include    <plugin/plugin.hh>

static minitl::ref<BugEngine::OpenGL::GLRenderer> create(const BugEngine::Plugin::Context &context)
{
    minitl::ref<BugEngine::OpenGL::GLRenderer> renderer;
    renderer = minitl::ref<BugEngine::OpenGL::GLRenderer>::create(BugEngine::Arena::game(), context);
    if (!renderer->success())
    {
        renderer = minitl::ref<BugEngine::OpenGL::GLRenderer>();
    }
    return renderer;
}

BE_PLUGIN_REGISTER_CREATE(&create);
