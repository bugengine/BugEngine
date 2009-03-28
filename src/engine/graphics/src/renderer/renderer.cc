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
        Assert(createRenderBackend);
        m_renderBackend = (*createRenderBackend)();
    }
}

Renderer::~Renderer()
{
    if(m_plugin)
    {
        void (*destroyRenderBackend)(RenderBackend*) = m_plugin.get<void (*)(RenderBackend*)>("destroyRenderBackend");
        Assert(destroyRenderBackend);
        (*destroyRenderBackend)(m_renderBackend);
    }
}

uint2 Renderer::getScreenSize()
{
    return m_renderBackend->getScreenSize();;
}

RenderTarget* Renderer::createRenderWindow(WindowFlags flags, const Scene* scene)
{
    return m_renderBackend->createRenderWindow(flags, scene);
}

GpuBuffer* Renderer::createVertexBuffer(size_t vertexCount, VertexUsage usage, VertexBufferFlags flags) const
{
    return m_renderBackend->createVertexBuffer(vertexCount, usage, flags);
}

GpuBuffer* Renderer::createIndexBuffer(size_t vertexCount, IndexUsage usage, IndexBufferFlags flags) const
{
    return m_renderBackend->createIndexBuffer(vertexCount, usage, flags);
}

GpuBuffer* Renderer::createTextureBuffer(TextureBufferFlags flags) const
{
    return m_renderBackend->createTextureBuffer(flags);
}

ShaderPipeline* Renderer::getShaderPipeline()
{
    return m_renderBackend->getShaderPipeline();
}

TexturePipeline* Renderer::getTexturePipeline()
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
