/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_WINDOWING_WINDOW_HH_
#define BE_WINDOWING_WINDOW_HH_
/*****************************************************************************/
#include    <3d/renderer/irendertarget.hh>
#include    <3d/renderer/irenderer.hh>

namespace BugEngine { namespace Windowing
{

class Renderer;

class be_api(WINDOWING) Window : public IRenderTarget
{
public:
    class PlatformWindow;
private:
    scoped<PlatformWindow>  m_window;
protected:
    void* getWindowHandle() const;
public:
    Window(weak<const RenderWindowDescription> renderWindowDescription, weak<const Renderer> renderer);
    ~Window();

    void load(weak<const Resource::Description> renderWindowDescription) override;
    void unload() override;

    uint2 getDimensions() const;
};

}}

/*****************************************************************************/
#endif
