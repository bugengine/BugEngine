/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/string/message.hh>
#include    <cstring>


namespace BugEngine
{

inline char* strdup(const char *src)
{
    if(!src)
        return 0;
    size_t ln = strlen(src);
    char* result = static_cast<char*>(be_malloc(ln+1));
    if(!result)
        return 0;
    return strncpy(result, src, ln+1);
}

message::message(const char *msg) :
    m_msg(strdup(msg))
{
}

message::~message()
{
    be_free(m_msg);
}

}
