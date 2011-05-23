/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_DX9_RENDERTARGETS_WINDOW_HH_
#define BE_DX9_RENDERTARGETS_WINDOW_HH_
/*****************************************************************************/
#include    <graphics/renderer/irenderer.hh>
#include    <renderer.hh>
#include    <windowing/window.hh>
#include    <d3d9.h>

namespace BugEngine { namespace Graphics { namespace DirectX9
{

class Window : public Windowing::Window
{
    friend class Renderer;
private:
    Renderer::SwapchainItem m_swapChain;
private:
    void setCurrent();
public:
    Window(weak<Renderer> renderer);
    ~Window();

    void begin(ClearMode clear) override;
    void end(PresentMode present) override;

    void close() override;
};

}}}

/*****************************************************************************/
#endif
