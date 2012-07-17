/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
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
