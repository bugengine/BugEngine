/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_NULLRENDER_LOADERS_NULLSURFACE_HH_
#define BE_NULLRENDER_LOADERS_NULLSURFACE_HH_
/**************************************************************************************************/
#include <stdafx.h>
#include <bugengine/plugin.graphics.3d/renderer/irenderer.hh>
#include <bugengine/plugin.graphics.3d/renderer/irendertarget.hh>

namespace BugEngine { namespace Null {

class NullRenderer;

class NullSurface : public IRenderTarget
{
public:
    NullSurface(weak< const RenderSurfaceDescription > resource,
                weak< const NullRenderer >             renderer);
    ~NullSurface();

private:
    void load(weak< const Resource::Description > surfaceDescription) override;
    void unload() override;

    void begin(ClearMode clear) const override;
    void end(PresentMode present) const override;

    void present() const;
};

}}  // namespace BugEngine::Null

/**************************************************************************************************/
#endif
