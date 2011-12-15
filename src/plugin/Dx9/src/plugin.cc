/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <dx9renderer.hh>
#include    <system/plugin.hh>

BE_PLUGIN_REGISTER(renderDx9, BugEngine::Graphics::IRenderer, BugEngine::Graphics::DirectX9::Dx9Renderer);
