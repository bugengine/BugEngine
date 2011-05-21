/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <renderer.hh>
#include    <window.hh>

#include    <system/resource/resource.script.hh>


namespace BugEngine { namespace Graphics { namespace OpenGL
{

Renderer::GPUResource::GPUResource(weak<const Resource> from)
    :   owner(from)
    ,   resource(0)
{
}

Renderer::GPUResource::~GPUResource()
{
    if (resource) unload();
}

void Renderer::GPUResource::load()
{
    be_assert(resource == 0, "GPU resource is already loaded!");
    resource = doLoad();
}

void Renderer::GPUResource::unload()
{
    be_assert(resource != 0, "GPU resource is not loaded!");
    doUnload();
    resource = 0;
}

ref<IRenderTarget> Renderer::createRenderWindow(WindowFlags flags)
{
    return ref<Window>::create(gameArena(), this, flags);
}

ref<IRenderTarget> Renderer::createRenderBuffer(TextureFlags /*flags*/)
{
    return ref<Window>();
}

ref<IRenderTarget> Renderer::createMultipleRenderBuffer(TextureFlags /*flags*/, size_t /*count*/)
{
    return ref<Window>();
}

void Renderer::flush()
{
    Windowing::Renderer::flush();
}

}}}
