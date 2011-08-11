/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_RENDERER_IRENDERTARGET_HH_
#define BE_GRAPHICS_RENDERER_IRENDERTARGET_HH_
/*****************************************************************************/
#include    <system/scheduler/task/group.hh>
#include    <3d/renderer/igpuresource.hh>

namespace BugEngine { namespace Graphics
{

class IRenderer;
class RenderTarget;

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
    IRenderTarget(weak<const RenderTarget> rendertarget, weak<IRenderer> renderer);
public:
    virtual ~IRenderTarget();

    weak<ITask>     syncTask() const;
    void            drawBatches(const Batch* batches, size_t count) const;
};

}}

/*****************************************************************************/
#endif
