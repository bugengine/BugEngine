/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <renderer.hh>

#include    <input/inputmap.hh>

#include    <window.hh>
#include    <texture.hh>
#include    <vertexbuffer.hh>
#include    <indexbuffer.hh>
#include    <texturebuffer.hh>


namespace BugEngine { namespace Graphics { namespace OpenGL
{

Renderer::Renderer()
:   m_glContext(0)
,   m_shaderPipeline(scoped<ShaderPipeline>::create(this))
,   m_texturePipeline(scoped<TexturePipeline>::create(this))
{
}

ref<RenderTarget> Renderer::createRenderWindow(WindowFlags flags)
{
    return ref<Window>::create(this, flags);
}

ref<GpuBuffer> Renderer::createVertexBuffer(u32 vertexCount, VertexUsage usage, VertexBufferFlags flags) const
{
    return ref<VertexBuffer>::create(this, vertexCount, usage, flags);
}

ref<GpuBuffer> Renderer::createIndexBuffer(u32 vertexCount, IndexUsage usage, IndexBufferFlags flags) const
{
    return ref<IndexBuffer>::create(this, vertexCount, usage, flags);
}

ref<GpuBuffer> Renderer::createTextureBuffer(TextureBufferFlags /*flags*/) const
{
    return ref<GpuBuffer>();
}

weak<Graphics::ShaderPipeline> Renderer::getShaderPipeline()
{
    return m_shaderPipeline;
}

weak<Graphics::TexturePipeline> Renderer::getTexturePipeline()
{
    return m_texturePipeline;
}

void Renderer::drawBatch(const Batch& b)
{
    weak<const VertexBuffer> _vb = be_checked_cast<const VertexBuffer>(b.vertices);
    weak<const IndexBuffer> _ib = be_checked_cast<const IndexBuffer>(b.indices);
    UNUSED(_vb);
    UNUSED(_ib);

    for(size_t i = 0; i < b.nbParams; ++i)
        b.params[i].first->setValue(b.params[i].second);

    switch(b.ptype)
    {
    case Batch::RptLineList:
        break;
    case Batch::RptTriangleList:
        break;
    case Batch::RptTriangleStrip:
    case Batch::RptTriangleFan:
    default:
        be_notreached();
    }
}

void Renderer::flush()
{
    OutputDebugString("flush\n");
    Thread::sleep(33);
}

}}}
