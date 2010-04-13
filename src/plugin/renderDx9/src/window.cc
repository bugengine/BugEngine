/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <window.hh>
#include    <renderer.hh>
#include    <cgshaderparam.hh>

namespace BugEngine
{
    extern HINSTANCE hDllInstance;
}


namespace BugEngine { namespace Graphics { namespace DirectX9
{

Window::Window(weak<Renderer> renderer, WindowFlags flags)
:   Win32::Window(renderer, flags)
{
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = !flags.fullscreen;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = m_window;
    d3dpp.BackBufferWidth = flags.size.x();
    d3dpp.BackBufferHeight = flags.size.y();
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.BackBufferCount = flags.vsync ? (flags.triplebuffered ? 3 : 2) : 2;
    d3dpp.PresentationInterval = flags.vsync ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE;

    m_swapChain = renderer->createSwapChain(&d3dpp);

    renderer->m_directx->AddRef();
    renderer->m_device->AddRef();
    setCurrent();
}

Window::~Window()
{
    be_checked_cast<Renderer>(m_renderer)->m_device->Release();
    be_checked_cast<Renderer>(m_renderer)->m_directx->Release();
}

bool Window::closed() const
{
    return m_swapChain == 0;
}

void Window::setCurrent()
{
    LPDIRECT3DSURFACE9 backBuffer;
    D3D_CHECKRESULT(m_swapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &backBuffer ));
    D3D_CHECKRESULT(be_checked_cast<Renderer>(m_renderer)->m_device->SetRenderTarget(0, backBuffer));
    backBuffer->Release();
}

void Window::close()
{
    m_swapChain->Release();
    m_swapChain = 0;
    Win32::Window::close();
}

void Window::begin()
{
    setCurrent();
    D3D_CHECKRESULT(be_checked_cast<Renderer>(m_renderer)->m_device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0));
    D3D_CHECKRESULT(be_checked_cast<Renderer>(m_renderer)->m_device->BeginScene());
    uint2 size = getDimensions();
    be_checked_cast<Renderer>(m_renderer)->m_systemParams[Renderer::__Screen]->setValue(float4(be_checked_numcast<float>(size.x()), be_checked_numcast<float>(size.y())));
}

void Window::end()
{
    D3D_CHECKRESULT(be_checked_cast<Renderer>(m_renderer)->m_device->EndScene());
    D3D_CHECKRESULT(m_swapChain->Present(NULL, NULL, NULL, NULL, 0));
}

}}}
