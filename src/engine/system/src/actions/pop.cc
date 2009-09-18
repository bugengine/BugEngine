/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <actions/pop.hh>

namespace BugEngine { namespace Actions
{

be_abstractmetaclass_impl("Actions",Pop);

Pop::Pop()
{
}

Pop::~Pop()
{
}

void Pop::initialize(ActionContext* /*context*/) const
{
    be_unimplemented();
}

bool Pop::oneturn(ActionContext* context) const
{
    ActionContext::StackFrame* frame = context->thisframe();
    frame->pop();
    return true;
}


}}
