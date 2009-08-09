/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

#include    <stdafx.h>
#include    <renderer.hh>

#include    <input/inputmap.hh>

#include    <resource.h>
#include    <window.hh>
#include    <texture.hh>
#include    <vertexbuffer.hh>
#include    <indexbuffer.hh>
#include    <texturebuffer.hh>
#include    <cgshader.hh>
#include    <cgshaderparam.hh>


namespace BugEngine { namespace Graphics { namespace OpenGL
{

static void onCgError(CGcontext /*ctx*/, CGerror /*err*/, void* /*data*/)
{

}

Renderer::Renderer()
:   m_glContext(0)
,   m_context(cgCreateContext())
,   m_shaderPipeline(this)
,   m_texturePipeline(this)
{
    cgSetErrorHandler(onCgError, 0);
}

RenderTarget* Renderer::createRenderWindow(WindowFlags flags, const Scene* scene)
{
    return new Window(this, flags, scene);
}

GpuBuffer* Renderer::createVertexBuffer(u32 vertexCount, VertexUsage usage, VertexBufferFlags flags) const
{
    return new VertexBuffer(this, vertexCount, usage, flags);
}

GpuBuffer* Renderer::createIndexBuffer(u32 vertexCount, IndexUsage usage, IndexBufferFlags flags) const
{
    return new IndexBuffer(this, vertexCount, usage, flags);
}

GpuBuffer* Renderer::createTextureBuffer(TextureBufferFlags /*flags*/) const
{
    return 0;
}

ShaderPipeline* Renderer::getShaderPipeline()
{
    return &m_shaderPipeline;
}

TexturePipeline* Renderer::getTexturePipeline()
{
    return &m_texturePipeline;
}

DebugRenderer* Renderer::debugRenderer()
{
    return m_debugRenderer.get();
}

void Renderer::createDebugRenderer()
{
    Assert(!m_debugRenderer);
    m_debugRenderer.reset(new DebugRenderer(this));
}

void Renderer::drawBatch(const Batch& b)
{
    const VertexBuffer* _vb = static_cast<const VertexBuffer*>(b.vertices);
    const IndexBuffer* _ib = static_cast<const IndexBuffer*>(b.indices);
    UNUSED(_vb);
    UNUSED(_ib);

    for(size_t i = 0; i < b.nbParams; ++i)
        b.params[i].first->setValue(b.params[i].second);

    if(b.vertexShader) static_cast<const CgShader*>(b.vertexShader)->set();
    if(b.pixelShader) static_cast<const CgShader*>(b.pixelShader)->set();

    switch(b.ptype)
    {
    case Batch::RptLineList:
        break;
    case Batch::RptTriangleList:
        break;
    case Batch::RptTriangleStrip:
    case Batch::RptTriangleFan:
    default:
        AssertNotReached();
    }
}

void Renderer::flush() const
{
}

}}}
