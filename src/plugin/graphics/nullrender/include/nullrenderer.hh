/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_NULLRENDER_RENDERER_HH_
#define BE_NULLRENDER_RENDERER_HH_
/**************************************************************************************************/
#include <stdafx.h>
#include <bugengine/filesystem/folder.script.hh>
#include <bugengine/plugin.graphics.3d/renderer/irenderer.hh>
#include <bugengine/plugin/plugin.hh>

namespace BugEngine { namespace Null {

class NullRenderer : public IRenderer
{
    BE_NOCOPY(NullRenderer);

public:
    NullRenderer(const Plugin::Context& context);
    ~NullRenderer();

    u32 getMaxSimultaneousRenderTargets() const override
    {
        return 1;
    }

    void  flush() override;
    uint2 getScreenSize() const override
    {
        return make_uint2(1920, 1080);
    }

private:
    ref< IGPUResource >
    create(weak< const RenderSurfaceDescription > renderSurfaceDescription) const override;
    ref< IGPUResource >
    create(weak< const RenderWindowDescription > renderWindowDescription) const override;
    ref< IGPUResource >
    create(weak< const ShaderProgramDescription > shaderDescription) const override;
};

}}  // namespace BugEngine::Null

/**************************************************************************************************/
#endif
