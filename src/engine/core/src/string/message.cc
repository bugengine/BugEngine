/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/string/message.hh>
#include    <cstring>


namespace BugEngine
{

message::message(const char *msg)
:   m_msg(msg)
{
}

message::~message()
{
}

}
