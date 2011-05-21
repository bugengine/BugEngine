/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <renderer.hh>
#include    <extensions.hh>
#include    <loaders/rendertarget/glrendertarget.hh>
#include    <graphics/objects/rendertarget.script.hh>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

GLRenderTarget::GLRenderTarget(weak<const Resource> resource)
    :   IGPUResource(resource)
{
}

GLRenderTarget::~GLRenderTarget()
{
}

void GLRenderTarget::load(weak<IRenderer> renderer)
{
}

void GLRenderTarget::unload(weak<IRenderer> renderer)
{
}

}}}
