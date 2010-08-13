/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <renderer.hh>

#include    <input/inputmap.hh>

#include    <rendertargets/window.hh>
#include    <rendertargets/offscreen.hh>
#include    <rendertargets/multiple.hh>
#include    <texture.hh>
#include    <vertexbuffer.hh>
#include    <indexbuffer.hh>
#include    <texturebuffer.hh>

#pragma warning(disable:4311 4312 4355)

namespace BugEngine
{
    extern HINSTANCE hDllInstance;
}


namespace BugEngine { namespace Graphics { namespace DirectX9
{

static void onCgError(CGcontext /*ctx*/, CGerror err, void* /*data*/)
{
    if(cgD3D9Errors(err) == cgD3D9Failed)
    {
        be_assert(false, "CG D3D error : %s" | cgD3D9TranslateHRESULT(cgD3D9GetLastError()));
    }
    else
    {
        be_assert(false, "CG error : %s" | cgD3D9TranslateCGerror(err));
    }
}

Renderer::Renderer(weak<const FileSystem> filesystem)
:   m_directx(Direct3DCreate9(D3D_SDK_VERSION))
,   m_device(0)
,   m_context(cgCreateContext())
,   m_filesystem(filesystem)
{
    cgSetErrorHandler(onCgError, 0);
    m_directx->GetDeviceCaps(0, D3DDEVTYPE_HAL, &m_caps);
}

Renderer::~Renderer()
{
    cgDestroyContext(m_context);
    cgD3D9SetDevice(0);
    if(m_device)
    {
        int refCnt = m_device->Release();
        be_forceuse(refCnt);
        be_assert(refCnt == 0, "device refcount is not 0");
    }
    int refCnt = m_directx->Release();
    be_forceuse(refCnt);
    be_assert(refCnt == 0, "Dx refcount is not 0");
}

LPDIRECT3DSWAPCHAIN9 Renderer::createSwapChain(D3DPRESENT_PARAMETERS* params)
{
    LPDIRECT3DSWAPCHAIN9 result;
    if(m_device)
    {
        D3D_CHECKRESULT(m_device->CreateAdditionalSwapChain(params, &result));
    }
    else
    {
        D3D_CHECKRESULT(m_directx->CreateDevice( D3DADAPTER_DEFAULT,
                                                 D3DDEVTYPE_HAL,
                                                 params->hDeviceWindow,
                                                 D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                                 params,
                                                 &m_device));
        cgD3D9SetDevice(m_device);
        D3D_CHECKRESULT(m_device->GetSwapChain(0, &result));
        D3D_CHECKRESULT(m_device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE));
        D3D_CHECKRESULT(m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE));
        D3D_CHECKRESULT(m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE));
        D3D_CHECKRESULT(m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA));
        D3D_CHECKRESULT(m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE));
    }
    return result;
}

ref<IRenderTarget> Renderer::createRenderWindow(WindowFlags flags)
{
    return ref<Window>::create(this, flags);
}

ref<IRenderTarget> Renderer::createRenderBuffer(TextureFlags /*flags*/)
{
    return ref<Window>();
}

ref<IRenderTarget> Renderer::createMultipleRenderBuffer(TextureFlags /*flags*/, size_t /*count*/)
{
    return ref<Window>();
}

ref<GpuBuffer> Renderer::createVertexBuffer(u32 vertexCount, VertexUsage usage, VertexBufferFlags flags) const
{
    return ref<VertexBuffer>::create(this, vertexCount, usage, flags);
}

ref<GpuBuffer> Renderer::createIndexBuffer(u32 vertexCount, IndexUsage usage, IndexBufferFlags flags) const
{
    return ref<IndexBuffer>::create(this, vertexCount, usage, flags);
}

void Renderer::drawBatch(const Batch& b)
{
    weak<const VertexBuffer> _vb = be_checked_cast<const VertexBuffer>(b.vertices);
    weak<const IndexBuffer> _ib = be_checked_cast<const IndexBuffer>(b.indices);

    D3D_CHECKRESULT(m_device->SetVertexDeclaration(_vb->m_vertexDecl));
    D3D_CHECKRESULT(m_device->SetStreamSource(0, _vb->m_buffer, 0, _vb->m_vertexStride));
    D3D_CHECKRESULT(m_device->SetIndices(_ib->m_buffer));

    D3DPRIMITIVETYPE type;
    int primitiveCount = 0;
    switch(b.ptype)
    {
    case Batch::RptLineList:
        type = D3DPT_LINELIST;
        primitiveCount = b.nbVertices/2;
        break;
    case Batch::RptTriangleList:
        type = D3DPT_TRIANGLELIST;
        primitiveCount = b.nbVertices/3;
        break;
    case Batch::RptTriangleStrip:
    case Batch::RptTriangleFan:
    default: be_notreached(); type = D3DPT_TRIANGLELIST; primitiveCount = b.nbVertices;
    }
    D3D_CHECKRESULT(m_device->DrawIndexedPrimitive(type, 0, 0, b.nbVertices, 0, primitiveCount));
}

void Renderer::flush()
{
    OutputDebugString("flush\n");
}

}}}
