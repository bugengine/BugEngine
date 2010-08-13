/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_RENDERER_GRAPH_INODE_HH_
#define BE_GRAPHICS_RENDERER_GRAPH_INODE_HH_
/*****************************************************************************/

namespace BugEngine
{

class ITask;

}

namespace BugEngine { namespace Graphics
{

class INode : public minitl::refcountable
{
public:
    INode();
    virtual ~INode();

    virtual weak<ITask> renderTask() = 0;
    virtual weak<ITask> dispatchTask() = 0;
    virtual bool closed() const = 0;
    virtual void disconnect() = 0;
};

}}

/*****************************************************************************/
#endif
