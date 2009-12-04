/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/renderer/renderer.hh>

namespace BugEngine { namespace Graphics
{

Renderer::Renderer(const istring& name)
:   m_plugin(name)
{
    if(m_plugin)
    {
        RenderBackend* (*createRenderBackend)() = m_plugin.get<RenderBackend* (*)()>("createRenderBackend");
        be_assert(createRenderBackend, "no function to create render backend in plugin");
        m_renderBackend = (*createRenderBackend)();
    }
}

Renderer::~Renderer()
{
    if(m_plugin)
    {
        void (*destroyRenderBackend)(RenderBackend*) = m_plugin.get<void (*)(RenderBackend*)>("destroyRenderBackend");
        RenderBackend* r = m_renderBackend.operator->();
        m_renderBackend.clear();
        be_assert(destroyRenderBackend, "no function to destroy render backend in plugin");
        (*destroyRenderBackend)(r);
    }
}

uint2 Renderer::getScreenSize()
{
    return m_renderBackend->getScreenSize();;
}

ref<RenderTarget> Renderer::createRenderWindow(WindowFlags flags, ref<const Scene> scene)
{
    return m_renderBackend->createRenderWindow(flags, scene);
}

ref<GpuBuffer> Renderer::createVertexBuffer(u32 vertexCount, VertexUsage usage, VertexBufferFlags flags) const
{
    return m_renderBackend->createVertexBuffer(vertexCount, usage, flags);
}

ref<GpuBuffer> Renderer::createIndexBuffer(u32 vertexCount, IndexUsage usage, IndexBufferFlags flags) const
{
    return m_renderBackend->createIndexBuffer(vertexCount, usage, flags);
}

ref<GpuBuffer> Renderer::createTextureBuffer(TextureBufferFlags flags) const
{
    return m_renderBackend->createTextureBuffer(flags);
}

weak<ShaderPipeline> Renderer::getShaderPipeline()
{
    return m_renderBackend->getShaderPipeline();
}

weak<TexturePipeline> Renderer::getTexturePipeline()
{
    return m_renderBackend->getTexturePipeline();
}

int Renderer::step() const
{
    return m_renderBackend->step();
}

void Renderer::flush() const
{
    return m_renderBackend->flush();
}

void Renderer::drawBatch(const Batch& batch)
{
    return m_renderBackend->drawBatch(batch);
}

}}
