/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/core/stdafx.h>
#include <bugengine/core/md5.hh>

extern "C"
{
#include <md5/md5.h>
}

namespace BugEngine {

MD5 digest(const void* buffer, u64 size)
{
    MD5_CTX context;
    MD5Init(&context);
    MD5Update(&context, (unsigned char*)buffer, be_checked_numcast< unsigned int >(size));
    MD5Final(&context);
    MD5 result = {{0, 0, 0, 0}};

    for(int i = 0; i < 4; ++i)
        result.hash[i] = context.digest[i * 4 + 0] << 0 | context.digest[i * 4 + 1] << 8
                         | context.digest[i * 4 + 2] << 16 | context.digest[i * 4 + 3] << 24;
    return result;
}

}  // namespace BugEngine
