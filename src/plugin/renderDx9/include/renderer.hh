/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_DX9_RENDERER_HH_
#define BE_DX9_RENDERER_HH_
/*****************************************************************************/
#include    <win32/renderer.hh>
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
    scoped<ShaderPipeline>      m_shaderPipeline;
    scoped<TexturePipeline>     m_texturePipeline;
    ref<CgShaderParam>          m_systemParams[SystemParamCount];
public:
    Renderer();
    ~Renderer();

    LPDIRECT3DSWAPCHAIN9    createSwapChain(D3DPRESENT_PARAMETERS* params);

    weak<ShaderPipeline>    getShaderPipeline() override;
    weak<TexturePipeline>   getTexturePipeline() override;

    ref<RenderTarget>       createRenderWindow(WindowFlags flags, weak<const Scene> scene) override;
    ref<GpuBuffer>          createVertexBuffer(u32 vertexCount, VertexUsage usage, VertexBufferFlags flags) const override;
    ref<GpuBuffer>          createIndexBuffer(u32 vertexCount, IndexUsage usage, IndexBufferFlags flags) const override;
    ref<GpuBuffer>          createTextureBuffer(TextureBufferFlags flags) const override;

    void                    drawBatch(const Batch& b) override;
    void                    flush() const override;
};

}}}

/*****************************************************************************/
#endif
