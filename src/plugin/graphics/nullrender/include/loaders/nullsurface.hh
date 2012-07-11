/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_NULLRENDER_LOADERS_NULLSURFACE_HH_
#define BE_NULLRENDER_LOADERS_NULLSURFACE_HH_
/*****************************************************************************/
#include    <3d/renderer/irendertarget.hh>
#include    <3d/renderer/irenderer.hh>


namespace BugEngine { namespace Null
{

class NullRenderer;

class NullSurface : public IRenderTarget
{
public:
    NullSurface(weak<const RenderSurface> resource, weak<const NullRenderer> renderer);
    ~NullSurface();

private:
    void load(weak<const Resource> resource) override;
    void unload() override;

    void begin(ClearMode clear) const override;
    void end(PresentMode present) const override;

    void present() const;
};

}}

/*****************************************************************************/
#endif
