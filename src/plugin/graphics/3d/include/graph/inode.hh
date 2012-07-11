/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_RENDERER_GRAPH_INODE_HH_
#define BE_GRAPHICS_RENDERER_GRAPH_INODE_HH_
/*****************************************************************************/
#include    <system/scheduler/task/itask.hh>

namespace BugEngine
{

namespace Task
{
class ITask;
}

class INode : public minitl::refcountable
{
    BE_NOCOPY(INode);
protected:
    INode();
public:
    virtual ~INode();

    virtual weak<Task::ITask> updateTask() = 0;
    virtual weak<Task::ITask> dispatchTask() = 0;
};

}

/*****************************************************************************/
#endif
