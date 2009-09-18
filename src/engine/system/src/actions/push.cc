/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <actions/push.hh>

namespace BugEngine { namespace Actions
{

be_abstractmetaclass_impl("Actions",Push);

Push::Push() :
    m_value()
{
}

Push::Push(const Value& value) :
    m_value(value)
{
}

Push::~Push()
{
}

refptr<Push> Push::MetaClass::createWithValue(const Value& value)
{
    return new Push(value);
}

void Push::initialize(ActionContext* context) const
{
    UNUSED(context);
}

bool Push::oneturn(ActionContext* context) const
{
    ActionContext::StackFrame* frame = context->thisframe();
    frame->push(m_value);
    return true;
}

}}
