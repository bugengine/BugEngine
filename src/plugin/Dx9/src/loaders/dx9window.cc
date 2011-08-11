/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <loaders/dx9window.hh>
#include    <dx9renderer.hh>
#include    <3d/rendertarget/rendertarget.script.hh>

namespace BugEngine
{
    extern HINSTANCE hDllInstance;
}


namespace BugEngine { namespace Graphics { namespace DirectX9
{


Dx9Window::Dx9Window(weak<const RenderWindow> resource, weak<Dx9Renderer> renderer)
:   Windowing::Window(resource, renderer)
,   m_swapChain(0)
{
}

Dx9Window::~Dx9Window()
{
    be_assert(m_swapChain == 0, "Dx9 render target destroyed but swapchain has not been unloaded properly");
}

void Dx9Window::setCurrent() const
{
    be_assert(Thread::currentId() == be_checked_cast<Dx9Renderer>(m_renderer)->m_threadId, "render command on wrong thread");
    if (m_swapChain)
    {
        LPDIRECT3DSURFACE9 backBuffer;
        d3d_checkResult(m_swapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &backBuffer ));
        d3d_checkResult(be_checked_cast<Dx9Renderer>(m_renderer)->m_device->SetRenderTarget(0, backBuffer));
        backBuffer->Release();
    }
}

void Dx9Window::begin(ClearMode clear) const
{
    be_assert(Thread::currentId() == be_checked_cast<Dx9Renderer>(m_renderer)->m_threadId, "render command on wrong thread");
    if (m_swapChain)
    {
        setCurrent();
        if (clear == Clear)
        {
            d3d_checkResult(be_checked_cast<Dx9Renderer>(m_renderer)->m_device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0));
        }
        d3d_checkResult(be_checked_cast<Dx9Renderer>(m_renderer)->m_device->BeginScene());
    }
}

void Dx9Window::end(PresentMode present) const
{
    be_assert(Thread::currentId() == be_checked_cast<Dx9Renderer>(m_renderer)->m_threadId, "render command on wrong thread");
    if (m_swapChain)
    {
        d3d_checkResult(be_checked_cast<Dx9Renderer>(m_renderer)->m_device->EndScene());
        if (present == Present)
        {
            if (d3d_checkResult(m_swapChain->Present(NULL, NULL, NULL, NULL, 0)) == D3DERR_DEVICELOST)
                be_checked_cast<Dx9Renderer>(m_renderer)->m_deviceState = Dx9Renderer::DeviceLost;
        }
    }
}

void Dx9Window::load(weak<const Resource> resource)
{
    be_assert(Thread::currentId() == be_checked_cast<Dx9Renderer>(m_renderer)->m_threadId, "render command on wrong thread");
    Window::load(resource);
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = true;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = *(HWND*)getWindowHandle();
    d3dpp.BackBufferWidth = 800;
    d3dpp.BackBufferHeight = 600;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.BackBufferCount = 1;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    d3d_checkResult(be_checked_cast<Dx9Renderer>(m_renderer)->m_device->CreateAdditionalSwapChain(&d3dpp, &m_swapChain));
}

void Dx9Window::unload()
{
    be_assert(Thread::currentId() == be_checked_cast<Dx9Renderer>(m_renderer)->m_threadId, "render command on wrong thread");
    Window::unload();
    m_swapChain->Release();
    m_swapChain = 0;
}


}}}
