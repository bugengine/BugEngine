/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/core/stdafx.h>
#include <bugengine/core/string/message.hh>

#include <cstring>

namespace BugEngine {

message::message(const char* msg) : m_msg(msg)
{
}

message::~message()
{
}

}  // namespace BugEngine
