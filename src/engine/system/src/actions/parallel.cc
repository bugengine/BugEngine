/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <actions/parallel.hh>

namespace BugEngine { namespace Actions
{

be_abstractmetaclass_impl("Actions",Parallel);

Parallel::Parallel()
{
}

Parallel::~Parallel()
{
}

void Parallel::initialize(ActionContext* /*context*/) const
{
    be_unimplemented();
}

bool Parallel::oneturn(ActionContext* /*context*/) const
{
    return true;
}

}}
