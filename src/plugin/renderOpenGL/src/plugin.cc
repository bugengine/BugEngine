/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <renderer.hh>

extern "C" FORCEEXPORT BugEngine::Graphics::RenderBackend* be_createPlugin()
{
    return new BugEngine::Graphics::OpenGL::Renderer;
}

extern "C" FORCEEXPORT void be_destroyPlugin(BugEngine::Graphics::RenderBackend* backend)
{
    delete be_checked_cast<BugEngine::Graphics::OpenGL::Renderer>(backend);
}

