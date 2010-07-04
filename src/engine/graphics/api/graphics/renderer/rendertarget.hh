/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_RENDERTARGET_HH_
#define BE_GRAPHICS_RENDERTARGET_HH_
/*****************************************************************************/
#include   <system/scheduler/task/itask.hh>

namespace BugEngine { namespace Graphics
{

class RenderBackend;

class be_api(GRAPHICS) RenderTarget : public minitl::refcountable
{
protected:
    weak<RenderBackend> const   m_renderer;
public:
    RenderTarget(weak<RenderBackend> renderer);
    virtual ~RenderTarget() { }

    virtual uint2   getDimensions() const = 0;
    virtual void    close() = 0;

    weak<ITask> flushTask() const;
protected:
    virtual bool    closed() const = 0;
private:
    RenderTarget& operator=(const RenderTarget& other);
    RenderTarget(const RenderTarget& other);
};

}}

/*****************************************************************************/
#endif
