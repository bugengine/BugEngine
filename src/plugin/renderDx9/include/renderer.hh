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

#ifndef BE_DX9_RENDERER_HH_
#define BE_DX9_RENDERER_HH_
/*****************************************************************************/
#include    <win32/renderer.hh>
#include    <graphics/renderer/debugrenderer.hh>
#include    <cgshaderparam.hh>
#include    <d3d9.h>
#include    <Cg/cg.h>
#include    <Cg/cgD3D9.h>

#include    <shaderpipeline.hh>
#include    <texturepipeline.hh>

namespace BugEngine { namespace Graphics { namespace DirectX9
{

class Window;
class VertexBuffer;
class IndexBuffer;
class TextureBuffer;

class Renderer : public Win32::Renderer
{
    friend class Window;
    friend class VertexBuffer;
    friend class IndexBuffer;
    friend class TextureBuffer;
    friend class ShaderPipeline;
    friend class TexturePipeline;
private:
    enum
    {
        __Screen,
        SystemParamCount
    };
private:
    LPDIRECT3D9                 m_directx;
    LPDIRECT3DDEVICE9           m_device;
    CGcontext                   m_context;
    scopedptr<DebugRenderer>    m_debugRenderer;
    ShaderPipeline              m_shaderPipeline;
    TexturePipeline             m_texturePipeline;
    refptr<CgShaderParam>       m_systemParams[SystemParamCount];
public:
    Renderer();
    ~Renderer();

    LPDIRECT3DSWAPCHAIN9    createSwapChain(D3DPRESENT_PARAMETERS* params);

    ShaderPipeline*         getShaderPipeline() override;
    TexturePipeline*        getTexturePipeline() override;

    RenderTarget*           createRenderWindow(WindowFlags flags, const Scene* scene) override;
    GpuBuffer*              createVertexBuffer(u32 vertexCount, VertexUsage usage, VertexBufferFlags flags) const override;
    GpuBuffer*              createIndexBuffer(u32 vertexCount, IndexUsage usage, IndexBufferFlags flags) const override;
    GpuBuffer*              createTextureBuffer(TextureBufferFlags flags) const override;

    DebugRenderer*          debugRenderer();

    void                    drawBatch(const Batch& b) override;
    void                    flush() const override;
private:
    void                    createDebugRenderer();
};

}}}

/*****************************************************************************/
#endif
