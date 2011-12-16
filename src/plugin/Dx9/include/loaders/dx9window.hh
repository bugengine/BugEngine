/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_DX9_LOADERS_DX9WINDOW_HH_
#define BE_DX9_LOADERS_DX9WINDOW_HH_
/*****************************************************************************/
#include    <dx9renderer.hh>
#include    <windowing/window.hh>
#include    <d3d9.h>

namespace BugEngine { namespace Graphics { namespace DirectX9
{

class Dx9Window : public Windowing::Window
{
    BE_NOCOPY(Dx9Window);
private:
    LPDIRECT3DSWAPCHAIN9 m_swapChain;
private:
    void setCurrent() const;

    void load(weak<const Resource> resource) override;
    void unload() override;

    void begin(ClearMode clear) const override;
    void end(PresentMode present) const override;
public:
    Dx9Window(weak<const RenderWindow> resource, weak<const Dx9Renderer> renderer);
    ~Dx9Window();
};

}}}

/*****************************************************************************/
#endif
