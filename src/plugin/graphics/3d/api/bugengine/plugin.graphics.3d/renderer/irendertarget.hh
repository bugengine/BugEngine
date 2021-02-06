/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_3D_RENDERER_IRENDERTARGET_HH_
#define BE_3D_RENDERER_IRENDERTARGET_HH_
/**************************************************************************************************/
#include <bugengine/plugin.graphics.3d/stdafx.h>
#include <bugengine/plugin.graphics.3d/renderer/igpuresource.hh>
#include <bugengine/scheduler/task/group.hh>

namespace BugEngine {

class IRenderer;
class RenderTargetDescription;

class be_api(3D) IRenderTarget : public IGPUResource
{
    BE_NOCOPY(IRenderTarget);

public:
    enum ClearMode
    {
        DontClear,
        Clear
    };
    enum PresentMode
    {
        DontPresent,
        Present
    };
    struct Batch
    {
    };

private:
    virtual void begin(ClearMode clear) const   = 0;
    virtual void end(PresentMode present) const = 0;

protected:
    IRenderTarget(weak< const RenderTargetDescription > rendertarget,
                  weak< const IRenderer >               renderer);

public:
    virtual ~IRenderTarget();

    weak< Task::ITask > syncTask() const;
    void                drawBatches(const Batch* batches, size_t count) const;
};

}  // namespace BugEngine

/**************************************************************************************************/
#endif
