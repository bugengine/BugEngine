/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_DX9_LOADERS_DX9WINDOW_HH_
#define BE_DX9_LOADERS_DX9WINDOW_HH_
/**************************************************************************************************/
#include <stdafx.h>
#include <bugengine/plugin.graphics.windowing/window.hh>
#include <d3d9.h>
#include <dx9renderer.hh>

namespace BugEngine { namespace DirectX9 {

class Dx9Window : public Windowing::Window
{
    BE_NOCOPY(Dx9Window);

private:
    LPDIRECT3DSWAPCHAIN9 m_swapChain;

private:
    void setCurrent() const;

    void load(weak< const Resource::Description > windowDescription) override;
    void unload() override;

    void begin(ClearMode clear) const override;
    void end(PresentMode present) const override;

public:
    Dx9Window(weak< const RenderWindowDescription > resource, weak< const Dx9Renderer > renderer);
    ~Dx9Window();
};

}}  // namespace BugEngine::DirectX9

/**************************************************************************************************/
#endif
