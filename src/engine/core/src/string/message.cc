/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/string/message.hh>
#include    <cstring>
#include    <libintl.h>


namespace BugEngine
{

message::message(const char *msg)
:   m_msg(gettext(msg))
{
}

message::~message()
{
}

}
