/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <actions/sequential.hh>

namespace BugEngine { namespace Actions
{

be_abstractmetaclass_impl("Actions",Sequential);

Sequential::Sequential()
{
}

Sequential::~Sequential()
{
}

void Sequential::initialize(ActionContext* context) const
{
    UNUSED(context);
}

bool Sequential::oneturn(ActionContext* /*context*/) const
{
    be_unimplemented();
    return true;
}

}}
