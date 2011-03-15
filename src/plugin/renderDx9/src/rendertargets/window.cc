/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <rendertargets/window.hh>
#include    <renderer.hh>

namespace BugEngine
{
    extern HINSTANCE hDllInstance;
}


namespace BugEngine { namespace Graphics { namespace DirectX9
{


Window::Window(weak<Renderer> renderer, WindowFlags flags)
:   Windowing::Window(renderer, flags)
{
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = !flags.fullscreen;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = *(HWND*)getWindowHandle();
    d3dpp.BackBufferWidth = flags.size.x();
    d3dpp.BackBufferHeight = flags.size.y();
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.BackBufferCount = flags.vsync ? (flags.triplebuffered ? 2 : 1) : 1;
    d3dpp.PresentationInterval = flags.vsync ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE;

    m_swapChain = be_checked_cast<Renderer>(m_renderer)->createSwapChain(d3dpp);
}

Window::~Window()
{
    m_swapChain = be_checked_cast<Renderer>(m_renderer)->release(m_swapChain);
}

void Window::setCurrent()
{
    if (m_swapChain->swapchain)
    {
        LPDIRECT3DSURFACE9 backBuffer;
        d3d_checkResult(m_swapChain->swapchain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &backBuffer ));
        d3d_checkResult(be_checked_cast<Renderer>(m_renderer)->m_device->SetRenderTarget(0, backBuffer));
        backBuffer->Release();
    }
}

void Window::begin(ClearMode clear)
{
    if (m_swapChain->swapchain)
    {
        setCurrent();
        if (clear == Clear)
        {
            d3d_checkResult(be_checked_cast<Renderer>(m_renderer)->m_device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0));
        }
        d3d_checkResult(be_checked_cast<Renderer>(m_renderer)->m_device->BeginScene());
    }
}

void Window::close()
{
    m_swapChain = be_checked_cast<Renderer>(m_renderer)->release(m_swapChain);
    Windowing::Window::close();
}

void Window::end(PresentMode present)
{
    if (m_swapChain->swapchain)
    {
        d3d_checkResult(be_checked_cast<Renderer>(m_renderer)->m_device->EndScene());
        if (present == Present)
        {
            if (d3d_checkResult(m_swapChain->swapchain->Present(NULL, NULL, NULL, NULL, 0)) == D3DERR_DEVICELOST)
                be_checked_cast<Renderer>(m_renderer)->m_deviceState = Renderer::DeviceLost;
        }
    }
}

}}}
