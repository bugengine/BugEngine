/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <actions/sleep.hh>

namespace BugEngine { namespace Actions
{

be_abstractmetaclass_impl("Actions",Sleep);

Sleep::Sleep()
{
}

Sleep::~Sleep()
{
}

void Sleep::initialize(ActionContext* context) const
{
    ActionContext::StackFrame frame(context, 1, 0);
    frame[0] = Value(0.0);
}

bool Sleep::oneturn(ActionContext* context) const
{
    ActionContext::StackFrame frame(context, 1, 0);
    double currenttime(frame[0].as<double>());
    currenttime += context->deltaTime();
    return currenttime >= frame[-1].as<double>();
}

}}
