/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_RENDERER_GRAPH_INODE_HH_
#define BE_GRAPHICS_RENDERER_GRAPH_INODE_HH_
/*****************************************************************************/
#include    <system/scheduler/task/itask.hh>

namespace BugEngine
{

class ITask;

}

namespace BugEngine { namespace Graphics
{

class INode : public minitl::refcountable
{
protected:
    class AsyncDispatchJobGraph
    {
        ITask::CallbackConnection   m_renderToSync;
        ITask::CallbackConnection   m_syncToRender;
        ITask::CallbackConnection   m_syncToDispatch;
        ITask::CallbackConnection   m_dispatchToSync;
    public:
        AsyncDispatchJobGraph(weak<ITask> renderTask, weak<ITask> syncTask, weak<ITask> dispatchTask);
    };
protected:
    INode();
public:
    virtual ~INode();

    virtual weak<ITask> updateTask() = 0;
    virtual weak<ITask> renderTask() = 0;
    virtual weak<ITask> syncTask() = 0;
    virtual weak<ITask> dispatchTask() = 0;
};

}}

/*****************************************************************************/
#endif
