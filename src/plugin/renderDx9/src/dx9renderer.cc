/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <dx9renderer.hh>

#include    <loaders/dx9window.hh>
#include    <loaders/dx9shader.hh>
#include    <graphics/objects/rendertarget.script.hh>
#include    <graphics/objects/shader.script.hh>



namespace BugEngine
{
    extern HINSTANCE hDllInstance;
}


namespace BugEngine { namespace Graphics { namespace DirectX9
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

Dx9Renderer::Dx9Renderer(weak<const FileSystem> filesystem)
:   Renderer(gameArena())
,   m_dummyWindow(CreateWindowEx(0, (minitl::format<>("__be__%p__") | (const void*)this).c_str(), "", WS_POPUP, 0, 0, 1, 1, 0, 0, hDllInstance, 0))
,   m_dummyParams(defaultParams(m_dummyWindow))
,   m_directx(Direct3DCreate9(D3D_SDK_VERSION))
,   m_device(0)
,   m_context(cgCreateContext())
,   m_filesystem(filesystem)
,   m_deviceState(DeviceLost)
,   m_threadId(Thread::currentId())
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

    static int frames = 0;
    static float now = Timer::now();
    if (++frames%100 == 0)
    {
        float time = Timer::now();
        be_debug("FPS: %d" | (int)(1000.0f*100.0f / (time - now)));
        now = time;
    }

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

ref<IGPUResource> Dx9Renderer::createRenderTarget(weak<const RenderTarget> rendertarget)
{
    return ref<IGPUResource>();
}

ref<IGPUResource> Dx9Renderer::createRenderWindow(weak<const RenderWindow> renderwindow)
{
    return ref<Dx9Window>::create(m_allocator, renderwindow, this);
}

ref<IGPUResource> Dx9Renderer::createShaderProgram(weak<const ShaderProgram> shader)
{
    return ref<Dx9ShaderProgram>::create(m_allocator, shader, this);
}

}}}
