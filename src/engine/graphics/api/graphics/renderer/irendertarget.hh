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
    IRenderTarget(weak<const RenderTarget> rendertarget, weak<const IRenderer> renderer);
    virtual ~IRenderTarget();

    virtual void    begin(ClearMode clear) = 0;
    virtual void    end(PresentMode present) = 0;

    weak<ITask>     syncTask() const;
    virtual bool    closed() const = 0;
private:
    IRenderTarget& operator=(const IRenderTarget& other);
    IRenderTarget(const IRenderTarget& other);
};

}}

/*****************************************************************************/
#endif
