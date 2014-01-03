/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_NULLRENDER_LOADERS_NULLWINDOW_HH_
#define BE_NULLRENDER_LOADERS_NULLWINDOW_HH_
/*****************************************************************************/
#include    <3d/renderer/irendertarget.hh>
#include    <3d/renderer/irenderer.hh>


namespace BugEngine { namespace Null
{

class NullRenderer;

class NullWindow : public IRenderTarget
{
public:
    NullWindow(weak<const RenderWindowDescription> windowDescription, weak<const NullRenderer> renderer);
    ~NullWindow();

private:
    void load(weak<const Resource::Description> windowDescription) override;
    void unload() override;

    void begin(ClearMode clear) const override;
    void end(PresentMode present) const override;

    void present() const;
};

}}

/*****************************************************************************/
#endif
