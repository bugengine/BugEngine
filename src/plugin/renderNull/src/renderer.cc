/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <renderer.hh>

#include    <window.hh>
#include    <loaders/nullmesh.hh>
#include    <loaders/nulltexture.hh>
#include    <loaders/nullshader.hh>

namespace BugEngine { namespace Graphics { namespace NullRenderer
{

void Renderer::flush()
{
}

Renderer::Renderer(weak<const FileSystem> filesystem)
:   m_filesystem(filesystem)
{
}

Renderer::~Renderer()
{
}

}}}

