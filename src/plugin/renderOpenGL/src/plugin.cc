/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <glrenderer.hh>
#include    <system/plugin.hh>

BE_PLUGIN_REGISTER(renderOpenGL, BugEngine::Graphics::OpenGL::GLRenderer, (weak<const BugEngine::FileSystem> fileSystem), (fileSystem));
