/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <input/stdafx.h>
#include    <input/control.hh>


namespace BugEngine { namespace Input
{

be_abstractmetaclass_impl("Input",Control);

Control::Control(const istring& name)
:   m_name(name)
{
}

Control::~Control()
{
}

const istring& Control::name() const
{
    return m_name;
}

}}
