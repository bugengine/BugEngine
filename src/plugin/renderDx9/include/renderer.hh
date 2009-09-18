/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

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
