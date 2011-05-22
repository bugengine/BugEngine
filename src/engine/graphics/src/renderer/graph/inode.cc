/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graph/inode.hh>


namespace BugEngine { namespace Graphics
{

INode::INode()
{
}

INode::~INode()
{
}

INode::AsyncDispatchJobGraph::AsyncDispatchJobGraph(weak<ITask> renderTask, weak<ITask> syncTask, weak<ITask> dispatchTask)
{
    m_renderToSync = ITask::CallbackConnection(renderTask, syncTask->startCallback());
    m_syncToRender = ITask::CallbackConnection(syncTask, renderTask->startCallback(), ITask::ICallback::Completed);
    m_syncToDispatch = ITask::CallbackConnection(syncTask, dispatchTask->startCallback());
    m_dispatchToSync = ITask::CallbackConnection(dispatchTask, syncTask->startCallback(), ITask::ICallback::Completed);
}

}}
