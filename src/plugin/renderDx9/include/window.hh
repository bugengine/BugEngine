/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_DX9_WINDOW_HH_
#define BE_DX9_WINDOW_HH_
/*****************************************************************************/
#include    <graphics/renderer/renderbackend.hh>
#include    <win32/window.hh>
#include    <d3d9.h>

namespace BugEngine { namespace Graphics { namespace DirectX9
{

class Renderer;

class Window : public Win32::Window
{
private:
    LPDIRECT3DSWAPCHAIN9    m_swapChain;
    LPDIRECT3DSWAPCHAIN9    m_workingSwapChain;
public:
    Window(weak<Renderer> renderer, WindowFlags flags);
    ~Window();

    void setCurrent();

    void begin();
    void end();

    void close() override;

    bool closed() const override;
};

}}}

/*****************************************************************************/
#endif
