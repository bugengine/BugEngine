/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <actions/call.hh>

namespace BugEngine { namespace Actions
{

be_abstractmetaclass_impl("Actions",Call);

Call::Call() :
    m_method(0)
{
}

Call::Call(RTTI::Method* method) :
    m_method(method)
{
}

Call::~Call()
{
}

refptr<Call> Call::MetaClass::createWithMethod(RTTI::Method* method)
{
    return new Call(method);
}

void Call::initialize(ActionContext* /*context*/) const
{
    be_unimplemented();
}

bool Call::oneturn(ActionContext* context) const
{
    ActionContext::StackFrame* frame = context->thisframe();
    size_t stackspace = frame->args();
    Value* wrapper = &(*frame)[0];
    Value result = m_method->metaclass()->call(wrapper, stackspace);
    frame->popall();
    frame->push(result);
    return true;
}

}}
