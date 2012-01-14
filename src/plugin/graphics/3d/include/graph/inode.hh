/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_RENDERER_GRAPH_INODE_HH_
#define BE_GRAPHICS_RENDERER_GRAPH_INODE_HH_
/*****************************************************************************/
#include    <system/scheduler/task/itask.hh>

namespace BugEngine
{

class ITask;

class INode : public minitl::refcountable
{
    BE_NOCOPY(INode);
protected:
    INode();
public:
    virtual ~INode();

    virtual weak<ITask> updateTask() = 0;
    virtual weak<ITask> dispatchTask() = 0;
};

}

/*****************************************************************************/
#endif
