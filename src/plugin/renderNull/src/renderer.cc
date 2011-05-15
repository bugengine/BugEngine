/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <renderer.hh>

#include    <window.hh>
#include    <loaders/meshloader.script.hh>
#include    <loaders/textureloader.script.hh>
#include    <loaders/shaderloader.script.hh>

namespace BugEngine { namespace Graphics { namespace NullRenderer
{

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
}

Renderer::Renderer(weak<const FileSystem> filesystem)
:   m_filesystem(filesystem)
,   m_meshLoader(scoped<MeshLoader>::create(gameArena()))
,   m_textureLoader(scoped<TextureLoader>::create(gameArena()))
,   m_shaderLoader(scoped<ShaderLoader>::create(gameArena()))
{
}

Renderer::~Renderer()
{
}

}}}

