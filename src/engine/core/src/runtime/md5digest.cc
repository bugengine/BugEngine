/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/runtime/md5.hh>

extern"C"
{
#include    <md5.h>
}

namespace BugEngine
{

MD5 digest(const void* buffer, size_t size)
{
    MD5_CTX context;
    MD5Init(&context);
    MD5Update(&context, (unsigned char*)buffer, be_checked_numcast<unsigned int>(size));
    MD5Final(&context);
    MD5 result;

    for (int i = 0; i < 4; ++i)
        result.hash[i] = context.digest[i*4+0] << 0
                | context.digest[i*4+1] << 8
                | context.digest[i*4+2] << 16
                | context.digest[i*4+3] << 24;
    return result;
}

}

