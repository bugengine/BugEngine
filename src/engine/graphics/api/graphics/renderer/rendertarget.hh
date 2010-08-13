/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_RENDERTARGET_HH_
#define BE_GRAPHICS_RENDERTARGET_HH_
/*****************************************************************************/
#include   <system/scheduler/task/group.hh>

namespace BugEngine { namespace Graphics
{

class RenderBackend;

class be_api(GRAPHICS) IRenderTarget : public minitl::refcountable
{
protected:
    weak<RenderBackend> const       m_renderer;
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
    IRenderTarget(weak<RenderBackend> renderer);
    virtual ~IRenderTarget();

    virtual uint2   getDimensions() const = 0;
    virtual void    close() = 0;
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
