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

struct CUSTOMVERTEX
{
    FLOAT x, y, z, rhw; // The transformed position for the vertex.
    DWORD color;        // The vertex color.
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)
LPDIRECT3DVERTEXBUFFER9 g_pVB;


Window::Window(weak<Renderer> renderer, WindowFlags flags)
:   Windowing::Window(renderer, flags)
,   m_closed(0)
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

    CUSTOMVERTEX vertices[] =
    {
        { 150.0f,  50.0f, 0.5f, 1.0f, 0xffff0000, }, // x, y, z, rhw, color
        { 250.0f, 250.0f, 0.5f, 1.0f, 0xff00ff00, },
        {  50.0f, 250.0f, 0.5f, 1.0f, 0xff00ffff, },
    };
    if(!g_pVB)
    {
        be_checked_cast<Renderer>(m_renderer)->m_device->CreateVertexBuffer(3*sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &g_pVB, NULL );
        VOID* pVertices;
        if( FAILED( g_pVB->Lock( 0, sizeof(vertices), (void**)&pVertices, 0 ) ) )
            return;
        memcpy( pVertices, vertices, sizeof(vertices) );
        g_pVB->Unlock();
    }
    else
    {
        g_pVB->AddRef();
    }
}

Window::~Window()
{
    m_swapChain = be_checked_cast<Renderer>(m_renderer)->release(m_swapChain);
}

bool Window::closed() const
{
    return m_closed > 0;
}

void Window::setCurrent()
{
    if(m_swapChain->swapchain)
    {
        LPDIRECT3DSURFACE9 backBuffer;
        d3d_checkResult(m_swapChain->swapchain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &backBuffer ));
        d3d_checkResult(be_checked_cast<Renderer>(m_renderer)->m_device->SetRenderTarget(0, backBuffer));
        backBuffer->Release();
    }
}

void Window::close()
{
    m_closed++;
}

void Window::begin(ClearMode clear)
{
    setCurrent();
    if(clear == Clear)
    {
        d3d_checkResult(be_checked_cast<Renderer>(m_renderer)->m_device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0));
    }
    d3d_checkResult(be_checked_cast<Renderer>(m_renderer)->m_device->BeginScene());

    be_checked_cast<Renderer>(m_renderer)->m_device->SetStreamSource( 0, g_pVB, 0, sizeof(CUSTOMVERTEX) );
    be_checked_cast<Renderer>(m_renderer)->m_device->SetFVF( D3DFVF_CUSTOMVERTEX );
    be_checked_cast<Renderer>(m_renderer)->m_device->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 1 );
}

void Window::end(PresentMode present)
{
    d3d_checkResult(be_checked_cast<Renderer>(m_renderer)->m_device->EndScene());
    if(m_closed > 0)
    {
        m_swapChain = be_checked_cast<Renderer>(m_renderer)->release(m_swapChain);
        Windowing::Window::close();
    }
    else if(present == Present && m_swapChain->swapchain)
    {
        if(d3d_checkResult(m_swapChain->swapchain->Present(NULL, NULL, NULL, NULL, 0)) == D3DERR_DEVICELOST)
            be_checked_cast<Renderer>(m_renderer)->m_deviceState = Renderer::DeviceLost;
    }
}

}}}
