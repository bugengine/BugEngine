/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_RENDERBACKEND_HH_
#define BE_GRAPHICS_RENDERBACKEND_HH_
/*****************************************************************************/
#include    <minitl/interlocked/stack.hh>
#include    <minitl/memory/pool.hh>
#include    <graphics/renderer/buffer/vertexdesc.hh>
#include    <graphics/scene/scene.hh>
#include    <graphics/renderer/rendertarget.hh>
#include    <graphics/renderer/buffer/gpubuffer.hh>
#include    <graphics/material/shader.hh>

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

enum RenderFormat
{
};

enum TextureFormat
{
    TfRGB_DXT1,
    TfRGBA_DXT5,
    TfRGBA_32,
    TfRGBA_128
};

struct TextureFlags
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
    RenderPrimitiveType                     ptype;
    const GpuBuffer*                        vertices;
    int                                     nbVertices;
    const GpuBuffer*                        indices;
};

class be_api(GRAPHICS) RenderBackend : public minitl::pointer
{
    friend class Renderer;
protected:
    RenderBackend();
    virtual ~RenderBackend();
protected:
    ref<ITask>              m_syncTask;
protected:
    virtual void flush() = 0;
public:
    weak<ITask>                     syncTask();

    virtual uint2                   getScreenSize() = 0;

    virtual u32                     getMaxSimultaneousRenderTargets() const = 0;

    virtual ref<IRenderTarget>      createRenderWindow(WindowFlags flags) = 0;
    virtual ref<IRenderTarget>      createRenderBuffer(TextureFlags flags) = 0;
    virtual ref<IRenderTarget>      createMultipleRenderBuffer(TextureFlags flags, size_t count) = 0;
    virtual ref<GpuBuffer>          createVertexBuffer(u32 vertexCount, VertexUsage usage, VertexBufferFlags flags) const = 0;
    virtual ref<GpuBuffer>          createIndexBuffer(u32 vertexCount, IndexUsage usage, IndexBufferFlags flags) const = 0;
};

}}

/*****************************************************************************/
#endif

