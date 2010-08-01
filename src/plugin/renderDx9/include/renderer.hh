/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_DX9_RENDERER_HH_
#define BE_DX9_RENDERER_HH_
/*****************************************************************************/
#include    <win32/renderer.hh>
#include    <cgshaderparam.hh>
#include    <system/filesystem.hh>
#include    <d3d9.h>
#include    <Cg/cg.h>
#include    <Cg/cgD3D9.h>

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
private:
    LPDIRECT3D9                 m_directx;
    LPDIRECT3DDEVICE9           m_device;
    D3DCAPS9                    m_caps;
    CGcontext                   m_context;
    weak<const FileSystem>      m_filesystem;
public:
    Renderer(weak<const FileSystem> filesystem);
    ~Renderer();

    LPDIRECT3DSWAPCHAIN9            createSwapChain(D3DPRESENT_PARAMETERS* params);

    ref<Graphics::IRenderTarget>    createRenderWindow(WindowFlags flags) override;
    ref<Graphics::IRenderTarget>    createRenderBuffer(TextureFlags flags) override;
    ref<Graphics::IRenderTarget>    createMultipleRenderBuffer(TextureFlags flags, size_t count) override;
    ref<GpuBuffer>                  createVertexBuffer(u32 vertexCount, VertexUsage usage, VertexBufferFlags flags) const override;
    ref<GpuBuffer>                  createIndexBuffer(u32 vertexCount, IndexUsage usage, IndexBufferFlags flags) const override;

    u32                             getMaxSimultaneousRenderTargets() const override { return m_caps.NumSimultaneousRTs; }

    weak<const FileSystem>          filesystem() const { return m_filesystem; }
private:
    void                            drawBatch(const Batch& b);
    void                            flush() override;
public:
    void* operator new(size_t size)                  { return ::operator new(size); }
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
public:
    void  operator delete(void* memory)              { return ::operator delete(memory); }
    void  operator delete(void* memory, void* where) { return ::operator delete(memory, where); }
};

}}}

/*****************************************************************************/
#endif
