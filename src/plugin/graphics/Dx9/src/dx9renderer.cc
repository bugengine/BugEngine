/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <dx9renderer.hh>

#include    <loaders/dx9window.hh>
#include    <loaders/dx9shader.hh>
#include    <3d/rendertarget/rendertarget.script.hh>
#include    <3d/shader/shader.script.hh>

#include    <core/threads/thread.hh>
#include    <system/plugin.hh>



namespace BugEngine
{
    extern HINSTANCE hDllInstance;
}


namespace BugEngine { namespace DirectX9
{

static D3DPRESENT_PARAMETERS defaultParams(HWND hwnd)
{
    D3DPRESENT_PARAMETERS params = {
        1,                      //BackBufferWidth
        1,                      //BackBufferHeight
        D3DFMT_UNKNOWN,         //BackBufferFormat
        0,                      //BackBufferCount
        D3DMULTISAMPLE_NONE,    //MultiSampleType
        0,                      //MultiSampleQuality
        D3DSWAPEFFECT_DISCARD,  //SwapEffect
        hwnd,                   //hDeviceWindow
        TRUE,                   //Windowed
        FALSE,                  //EnableAutoDepthStencil
        D3DFMT_UNKNOWN,         //AutoDepthStencilFormat
        0,                      //Flags
        0,                      //FullScreen_RefreshRateInHz
        0                       //PresentationInterval
    };
    return params;
}

Dx9Renderer::Dx9Renderer(const PluginContext& context)
:   Renderer(Arena::general(), context.resourceManager)
,   m_dummyWindow(CreateWindowEx(0, (minitl::format<128>("__be__%p__") | (const void*)this).c_str(), "", WS_POPUP, 0, 0, 1, 1, 0, 0, hDllInstance, 0))
,   m_dummyParams(defaultParams(m_dummyWindow))
,   m_directx(Direct3DCreate9(D3D_SDK_VERSION))
,   m_device(0)
,   m_context(cgCreateContext())
,   m_threadId(Thread::currentId())
,   m_deviceState(DeviceLost)
{
    m_directx->GetDeviceCaps(0, D3DDEVTYPE_HAL, &m_caps);

    d3d_checkResult(m_directx->CreateDevice( D3DADAPTER_DEFAULT,
                                             D3DDEVTYPE_HAL,
                                             m_dummyParams.hDeviceWindow,
                                             D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
                                             &m_dummyParams,
                                             &m_device));
    d3d_checkResult(m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE));
    d3d_checkResult(m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE));
    d3d_checkResult(m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA));
    d3d_checkResult(m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE));

    m_deviceState = DeviceReady;

    m_device->AddRef();
    m_directx->AddRef();
}

Dx9Renderer::~Dx9Renderer()
{
    DestroyWindow(m_dummyWindow);

    if (m_device)
    {
        int refCnt = m_device->Release();
        be_forceuse(refCnt);
        be_assert(refCnt == 0, "device refcount is not 0");
        m_device = 0;
    }
    if (m_directx)
    {
        int refCnt = m_directx->Release();
        be_forceuse(refCnt);
        be_assert(refCnt == 0, "Dx refcount is not 0");
    }
}

void Dx9Renderer::flush()
{
    Windowing::Renderer::flush();

    if (m_deviceState == DeviceLost)
    {
        // TODO: unload
        if (d3d_checkResult(m_device->TestCooperativeLevel()) != D3DERR_DEVICELOST)
        {
            if (!FAILED(d3d_checkResult(m_device->Reset(&m_dummyParams))))
            {
                m_deviceState = DeviceRestored;
            }
        }
    }
    if (m_deviceState == DeviceRestored)
    {
        // TODO: reload
        m_deviceState = DeviceReady;
    }
}

ref<IGPUResource> Dx9Renderer::create(weak<const RenderSurface> rendersurface) const
{
    return ref<IGPUResource>();
}

ref<IGPUResource> Dx9Renderer::create(weak<const RenderWindow> renderwindow) const
{
    return ref<Dx9Window>::create(m_allocator, renderwindow, this);
}

ref<IGPUResource> Dx9Renderer::create(weak<const ShaderProgram> shader) const
{
    return ref<Dx9ShaderProgram>::create(m_allocator, shader, this);
}

}}
