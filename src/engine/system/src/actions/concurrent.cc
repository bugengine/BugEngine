/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <actions/concurrent.hh>

namespace BugEngine { namespace Actions
{

be_abstractmetaclass_impl("Actions",Concurrent);

Concurrent::Concurrent()
{
}

Concurrent::~Concurrent()
{
}

void Concurrent::initialize(ActionContext* /*context*/) const
{
    be_unimplemented();
}

bool Concurrent::oneturn(ActionContext* /*context*/) const
{
    be_unimplemented();
    return true;
}

}}
