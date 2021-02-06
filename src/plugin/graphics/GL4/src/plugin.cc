/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.graphics.GL4/stdafx.h>
#include <bugengine/plugin.graphics.GL4/glrenderer.hh>
#include <bugengine/plugin/plugin.hh>

static minitl::ref< BugEngine::OpenGL::GLRenderer >
create(const BugEngine::Plugin::Context& context)
{
    minitl::ref< BugEngine::OpenGL::GLRenderer > renderer;
    renderer
        = minitl::ref< BugEngine::OpenGL::GLRenderer >::create(BugEngine::Arena::game(), context);
    if(!renderer->success())
    {
        renderer = minitl::ref< BugEngine::OpenGL::GLRenderer >();
    }
    return renderer;
}

BE_PLUGIN_REGISTER_CREATE(&create);
