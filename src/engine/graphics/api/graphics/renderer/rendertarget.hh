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
    ref<TaskGroup>                  m_flushTask;
    TaskGroup::TaskStartConnection  m_startFlushConnection;
    TaskGroup::TaskEndConnection    m_endFlushConnection;
public:
    IRenderTarget(weak<RenderBackend> renderer);
    virtual ~IRenderTarget();

    virtual uint2   getDimensions() const = 0;
    virtual void    close() = 0;
    virtual void    setCurrent() = 0;
    virtual void    begin() = 0;
    virtual void    end() = 0;

    weak<ITask>     flushTask() const;
    virtual bool    closed() const = 0;
private:
    IRenderTarget& operator=(const IRenderTarget& other);
    IRenderTarget(const IRenderTarget& other);
};

}}

/*****************************************************************************/
#endif
