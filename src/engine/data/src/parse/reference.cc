/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <data/stdafx.h>
#include    <parse/reference.hh>
#include    <parse/database.hh>

namespace BugEngine { namespace Data { namespace Parse
{

Reference::Reference(const inamespace& name) :
    m_reference(name)
{
}

Reference::~Reference()
{
}

void Reference::dolink(Context& /*context*/) const
{
}

Value Reference::doeval(Context& /*context*/) const
{
    return Value();
}

}}}
