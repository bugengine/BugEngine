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

#ifndef BE_GRAPHICS_RENDERER_HH_
#define BE_GRAPHICS_RENDERER_HH_
/*****************************************************************************/
#include    <system/plugin.hh>
#include    <graphics/renderer/renderbackend.hh>

namespace BugEngine { namespace Graphics
{

class GRAPHICSEXPORT Renderer : public RenderBackend
{
private:
    Plugin          m_plugin;
    RenderBackend*  m_renderBackend;
public:
    Renderer(const istring& name);
    ~Renderer();

    void flush() const override;
    int step() const override;

    uint2 getScreenSize() override;

    ShaderPipeline*     getShaderPipeline() override;
    TexturePipeline*    getTexturePipeline() override;

    RenderTarget*       createRenderWindow(WindowFlags flags, const Scene* scene) override;
    GpuBuffer*          createVertexBuffer(u32 vertexCount, VertexUsage usage, VertexBufferFlags flags) const override;
    GpuBuffer*          createIndexBuffer(u32 vertexCount, IndexUsage usage, IndexBufferFlags flags) const override;
    GpuBuffer*          createTextureBuffer(TextureBufferFlags flags) const override;

    void drawBatch(const Batch& batch) override;
};

}}

/*****************************************************************************/
#endif
