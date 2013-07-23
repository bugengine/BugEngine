/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_WINDOWING_RENDERER_HH_
#define BE_WINDOWING_RENDERER_HH_
/*****************************************************************************/
#include    <3d/renderer/irenderer.hh>
#include    <windowing/window.hh>

namespace BugEngine { namespace Windowing
{

class Window;
struct WindowCreationFlags;

	class be_api(WINDOWING) Renderer : public IRenderer
{
    friend class Window;
    friend class Window::PlatformWindow;
    BE_NOCOPY(Renderer);
private:
    class PlatformRenderer;
    friend class PlatformRenderer;
    scoped<PlatformRenderer>    m_platformRenderer;
protected:
    void            flush() override;
protected:
    void*           createDummyHandle();
public:
    Renderer(minitl::Allocator& allocator, weak<Resource::ResourceManager> manager);
    ~Renderer();

    uint2           getScreenSize() const override;
    void*           getPlatformData();
};

}}

/*****************************************************************************/
#endif
