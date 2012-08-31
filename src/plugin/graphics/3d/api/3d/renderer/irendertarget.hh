/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_3D_RENDERER_IRENDERTARGET_HH_
#define BE_3D_RENDERER_IRENDERTARGET_HH_
/*****************************************************************************/
#include    <scheduler/task/group.hh>
#include    <3d/renderer/igpuresource.hh>

namespace BugEngine
{

class IRenderer;
class RenderTargetDescription;

class be_api(_3D) IRenderTarget : public IGPUResource
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
    virtual void    begin(ClearMode clear) const = 0;
    virtual void    end(PresentMode present) const = 0;
protected:
    IRenderTarget(weak<const RenderTargetDescription> rendertarget, weak<const IRenderer> renderer);
public:
    virtual ~IRenderTarget();

    weak<Task::ITask>   syncTask() const;
    void                drawBatches(const Batch* batches, size_t count) const;
};

}

/*****************************************************************************/
#endif
