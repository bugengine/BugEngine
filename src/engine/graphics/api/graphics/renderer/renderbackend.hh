/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_RENDERBACKEND_HH_
#define BE_GRAPHICS_RENDERBACKEND_HH_
/*****************************************************************************/
#include    <minitl/interlocked/stack.hh>
#include    <minitl/memory/pool.hh>
#include    <graphics/renderer/vertexdesc.hh>
#include    <graphics/scene/scene.hh>
#include    <graphics/renderer/rendertarget.hh>
#include    <graphics/renderer/gpubuffer.hh>
#include    <graphics/material/shader.hh>
#include    <graphics/material/shaderpipeline.hh>
#include    <graphics/material/texturepipeline.hh>

namespace BugEngine { namespace Graphics
{

struct WindowFlags
{
    uint2   size;
    int2    position;
    istring title;
    bool    border;
    bool    fullscreen;
    bool    vsync;
    bool    triplebuffered;
};

enum TextureFormat
{
    TfRGB_DXT1,
    TfRGBA_DXT5,
    TfRGBA_32,
    TfRGBA_128
};

struct TextureBufferFlags
{
    uint2           size;
    TextureFormat   format;
};

struct Batch : public minitl::inode
{
    enum RenderPrimitiveType
    {
        RptLineList,
        RptTriangleList,
        RptTriangleStrip,
        RptTriangleFan
    };

    const RenderTarget*                     rtarget;
    RenderPrimitiveType                     ptype;
    std::pair< ShaderParam*, float4>        params[32];
    size_t                                  nbParams;
    const GpuBuffer*                        vertices;
    int                                     nbVertices;
    const GpuBuffer*                        indices;
    const Shader*                           vertexShader;
    const Shader*                           pixelShader;
};

class be_api(GRAPHICS) RenderBackend : public minitl::pointer
{
    friend class Renderer;
protected:
    RenderBackend();
    virtual ~RenderBackend();
protected:
    ref<ITask>              m_flushTask;
protected:
    virtual int step() const = 0;
    virtual void flush() = 0;
public:
    weak<ITask>                     flushTask();

    virtual uint2                   getScreenSize() = 0;

    virtual weak<ShaderPipeline>    getShaderPipeline() = 0;
    virtual weak<TexturePipeline>   getTexturePipeline() = 0;

    virtual ref<RenderTarget>       createRenderWindow(WindowFlags flags) = 0;
    virtual ref<GpuBuffer>          createVertexBuffer(u32 vertexCount, VertexUsage usage, VertexBufferFlags flags) const = 0;
    virtual ref<GpuBuffer>          createIndexBuffer(u32 vertexCount, IndexUsage usage, IndexBufferFlags flags) const = 0;
    virtual ref<GpuBuffer>          createTextureBuffer(TextureBufferFlags flags) const = 0;
};

}}

/*****************************************************************************/
#endif

