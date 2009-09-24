/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_RENDERER_HH_
#define BE_GRAPHICS_RENDERER_HH_
/*****************************************************************************/
#include    <system/plugin.hh>
#include    <graphics/renderer/renderbackend.hh>

namespace BugEngine { namespace Graphics
{

class be_api(GRAPHICS) Renderer : public RenderBackend
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
