/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/action.hh>

namespace BugEngine
{

static const size_t g_nbStackSlots = 30000;
ActionContext::ActionContext()
:   m_stack(g_nbStackSlots)
,   m_deltaTime(0.0)
,   m_currentFrame(0)
,   m_frameSize(30000)
,   m_frame(0)
{
}

ActionContext::~ActionContext()
{
}

//-----------------------------------------------------------------------------

ActionContext::StackFrame::StackFrame(ActionContext* context, size_t nbargs, size_t locals)
:   m_context(context)
,   m_previous(context->m_frame)
,   m_start(context->m_currentFrame)
,   m_size(context->m_frameSize)
,   m_local(locals)
,   m_caller(0)
,   m_callee(nbargs)
{
    be_assert(m_callee == m_previous->m_caller, "callee and caller should match");
    be_assert(m_start+m_size <= context->m_stack.size(), "stack overflow");
    m_context->m_frame = this;
    m_context->m_currentFrame += m_local + m_callee; 
}

ActionContext::StackFrame::~StackFrame()
{
    m_context->m_frame = m_previous;
    m_context->m_currentFrame -= m_local + m_callee;
}

void ActionContext::StackFrame::push(const Value& value)
{
    be_assert(m_start+m_caller+m_callee+m_local < m_context->m_stack.size(), "stack overflow");
    m_context->m_stack[m_start+m_caller+m_callee+m_local] = value;
    m_callee++;
}

void ActionContext::StackFrame::pop()
{
    m_callee--;
}

void ActionContext::StackFrame::popall()
{
    m_callee = 0;
}

size_t ActionContext::StackFrame::args() const
{
    return m_callee;
}

Value& ActionContext::StackFrame::operator[](int index)
{
    size_t realindex = m_start + index;
    be_assert(realindex < m_start+m_caller+m_callee+m_local, "index %d out of range %d" | realindex | (m_start+m_caller+m_callee+m_local));
    return m_context->m_stack[realindex];
}

//-----------------------------------------------------------------------------

be_abstractmetaclass_impl("",Action);

Action::Action()
{
}

Action::~Action()
{
}

}
