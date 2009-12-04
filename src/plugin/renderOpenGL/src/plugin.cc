/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <renderer.hh>

extern "C" FORCEEXPORT void _initplugin(void)
{
}

extern "C" FORCEEXPORT void _finiplugin(void)
{
}

extern "C" FORCEEXPORT BugEngine::Graphics::RenderBackend* createRenderBackend()
{
    return new BugEngine::Graphics::OpenGL::Renderer;
}

extern "C" FORCEEXPORT void destroyRenderBackend(BugEngine::Graphics::RenderBackend* backend)
{
    delete be_checked_cast<BugEngine::Graphics::OpenGL::Renderer>(backend);
}

