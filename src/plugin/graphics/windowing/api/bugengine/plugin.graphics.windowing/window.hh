/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_WINDOWING_WINDOW_HH_
#define BE_WINDOWING_WINDOW_HH_
/**************************************************************************************************/
#include <bugengine/plugin.graphics.windowing/stdafx.h>
#include <bugengine/plugin.graphics.3d/renderer/irenderer.hh>
#include <bugengine/plugin.graphics.3d/renderer/irendertarget.hh>

namespace BugEngine { namespace Windowing {

class Renderer;

class be_api(WINDOWING) Window : public IRenderTarget
{
public:
    class PlatformWindow;

private:
    scoped< PlatformWindow > m_window;

protected:
    void* getWindowHandle() const;

public:
    Window(weak< const RenderWindowDescription > renderWindowDescription,
           weak< const Renderer >                renderer);
    ~Window();

    void load(weak< const Resource::Description > renderWindowDescription) override;
    void unload() override;

    uint2 getDimensions() const;
};

}}  // namespace BugEngine::Windowing

/**************************************************************************************************/
#endif
