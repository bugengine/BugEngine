/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_RENDERER_IRENDERTARGET_HH_
#define BE_GRAPHICS_RENDERER_IRENDERTARGET_HH_
/*****************************************************************************/
#include    <system/scheduler/task/group.hh>
#include    <graphics/renderer/igpuresource.hh>

namespace BugEngine { namespace Graphics
{

class IRenderer;
class RenderTarget;

class be_api(GRAPHICS) IRenderTarget : public IGPUResource
{
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
private:
    IRenderTarget& operator=(const IRenderTarget& other);
    IRenderTarget(const IRenderTarget& other);
};

}}

/*****************************************************************************/
#endif
