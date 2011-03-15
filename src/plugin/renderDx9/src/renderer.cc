/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <renderer.hh>

#include    <input/inputmap.hh>

#include    <rendertargets/window.hh>
#include    <rendertargets/offscreen.hh>
#include    <rendertargets/multiple.hh>

#pragma warning(disable:4311 4312 4355)

namespace BugEngine
{
    extern HINSTANCE hDllInstance;
}


namespace BugEngine { namespace Graphics { namespace DirectX9
{

Renderer::Renderer(weak<const FileSystem> filesystem)
:   m_directx(Direct3DCreate9(D3D_SDK_VERSION))
,   m_device(0)
,   m_context(cgCreateContext())
,   m_filesystem(filesystem)
,   m_swapchains(gameArena())
,   m_deviceState(DeviceLost)
{
    m_directx->GetDeviceCaps(0, D3DDEVTYPE_HAL, &m_caps);
}

Renderer::~Renderer()
{
    destroyContextAsync();
    int refCnt = m_directx->Release();
    be_forceuse(refCnt);
    be_assert(refCnt == 0, "Dx refcount is not 0");
}

Renderer::SwapchainItem Renderer::createSwapChain(D3DPRESENT_PARAMETERS params)
{
    SwapchainItem it = m_swapchains.insert(m_swapchains.end(), SwapchainDesc());
    it->params = params;
    if (m_device)
    {
        d3d_checkResult(m_device->CreateAdditionalSwapChain(&it->params, &it->swapchain));
    }
    else
    {
        createContextAsync((void*)&params);
        m_deviceSwapChain = it;
        d3d_checkResult(m_device->GetSwapChain(0, &it->swapchain));
        //d3d_checkResult(m_device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE));
        d3d_checkResult(m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE));
        d3d_checkResult(m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE));
        d3d_checkResult(m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA));
        d3d_checkResult(m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE));
    }
    m_device->AddRef();
    m_directx->AddRef();
    return it;
}

void Renderer::createContext(void* params_)
{
    D3DPRESENT_PARAMETERS& params = *(D3DPRESENT_PARAMETERS*)params_;
    if (m_device)
    {
        if (!FAILED(d3d_checkResult(m_device->Reset(&params))))
        {
            m_deviceState = DeviceRestored;
        }
    }
    else
    {
        d3d_checkResult(m_directx->CreateDevice( D3DADAPTER_DEFAULT,
                                                 D3DDEVTYPE_HAL,
                                                 params.hDeviceWindow,
                                                 D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
                                                 &params,
                                                 &m_device));
        m_deviceState = DeviceReady;
    }
}

void Renderer::destroyContext()
{
    if (m_device)
    {
        int refCnt = m_device->Release();
        be_forceuse(refCnt);
        be_assert(refCnt == 0, "device refcount is not 0");
        m_device = 0;
    }
}

Renderer::SwapchainItem Renderer::release(SwapchainItem swapchain)
{
    if (swapchain != m_swapchains.end())
    {
        if (swapchain == m_deviceSwapChain)
        {
            m_deviceSwapChain =  m_swapchains.end();
            m_deviceState = DeviceLost;
        }
        swapchain->swapchain->Release();
        m_device->Release();
        m_directx->Release();
        m_swapchains.erase(swapchain);
    }
    return m_swapchains.end();
}

ref<IRenderTarget> Renderer::createRenderWindow(WindowFlags flags)
{
    return ref<Window>::create(gameArena(), this, flags);
}

ref<IRenderTarget> Renderer::createRenderBuffer(TextureFlags /*flags*/)
{
    return ref<Window>();
}

ref<IRenderTarget> Renderer::createMultipleRenderBuffer(TextureFlags /*flags*/, size_t /*count*/)
{
    return ref<Window>();
}

void Renderer::drawBatch(const Batch& b)
{
    //weak<const VertexBuffer> _vb = be_checked_cast<const VertexBuffer>(b.vertices);
    //weak<const IndexBuffer> _ib = be_checked_cast<const IndexBuffer>(b.indices);

    //d3d_checkResult(m_device->SetVertexDeclaration(_vb->m_vertexDecl));
    //d3d_checkResult(m_device->SetStreamSource(0, _vb->m_buffer, 0, _vb->m_vertexStride));
    //d3d_checkResult(m_device->SetIndices(_ib->m_buffer));

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
    d3d_checkResult(m_device->DrawIndexedPrimitive(type, 0, 0, b.nbVertices, 0, primitiveCount));
}

void Renderer::flush()
{
    Windowing::Renderer::flush();
    //static Timer t;
    //static int frames = 0;
    if (m_deviceState == DeviceLost)
    {
        for (minitl::list<SwapchainDesc>::iterator it = m_swapchains.begin(); it != m_swapchains.end(); ++it)
        {
            if (it->swapchain)
            {
                it->swapchain->Release();
                it->swapchain = 0;
            }
        }
        if (!m_swapchains.empty())
        {
            if (m_deviceSwapChain == m_swapchains.end())
            {
                m_deviceSwapChain = m_swapchains.begin();
            }
            if (d3d_checkResult(m_device->TestCooperativeLevel()) != D3DERR_DEVICELOST)
            {
                createContextAsync((void*)&m_deviceSwapChain->params);
            }
        }
        else
        {
            destroyContextAsync();
        }
    }
    if (m_deviceState == DeviceRestored)
    {
        if (!m_swapchains.empty())
        {
            d3d_checkResult(m_device->GetSwapChain(0, &m_deviceSwapChain->swapchain));
            for (minitl::list<SwapchainDesc>::iterator it = m_swapchains.begin(); it != m_swapchains.end(); ++it)
            {
                if (it != m_deviceSwapChain)
                {
                    d3d_checkResult(m_device->CreateAdditionalSwapChain(&it->params, &it->swapchain));
                }
            }
            m_deviceState = DeviceReady;
        }
    }
}

}}}
