/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <renderer.hh>
#include    <system/plugin.hh>

BE_PLUGIN_REGISTER(renderOpenGL, BugEngine::Graphics::OpenGL::Renderer, (weak<const BugEngine::FileSystem> fileSystem), (fileSystem));
