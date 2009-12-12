/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/renderer/renderer.hh>

namespace BugEngine { namespace Graphics
{

Renderer::Renderer(const istring& name)
:   m_plugin(name)
{
}

Renderer::~Renderer()
{
}

uint2 Renderer::getScreenSize()
{
    return m_plugin->getScreenSize();;
}

ref<RenderTarget> Renderer::createRenderWindow(WindowFlags flags, ref<const Scene> scene)
{
    return m_plugin->createRenderWindow(flags, scene);
}

ref<GpuBuffer> Renderer::createVertexBuffer(u32 vertexCount, VertexUsage usage, VertexBufferFlags flags) const
{
    return m_plugin->createVertexBuffer(vertexCount, usage, flags);
}

ref<GpuBuffer> Renderer::createIndexBuffer(u32 vertexCount, IndexUsage usage, IndexBufferFlags flags) const
{
    return m_plugin->createIndexBuffer(vertexCount, usage, flags);
}

ref<GpuBuffer> Renderer::createTextureBuffer(TextureBufferFlags flags) const
{
    return m_plugin->createTextureBuffer(flags);
}

weak<ShaderPipeline> Renderer::getShaderPipeline()
{
    return m_plugin->getShaderPipeline();
}

weak<TexturePipeline> Renderer::getTexturePipeline()
{
    return m_plugin->getTexturePipeline();
}

int Renderer::step() const
{
    return m_plugin->step();
}

void Renderer::flush() const
{
    return m_plugin->flush();
}

void Renderer::drawBatch(const Batch& batch)
{
    return m_plugin->drawBatch(batch);
}

}}
