/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_RUNTIME_MD5_HH_
#define BE_CORE_RUNTIME_MD5_HH_
/*****************************************************************************/

namespace BugEngine
{

struct MD5
{
    u32 hash[4];
};

static inline bool operator==(const MD5& hash1, const MD5& hash2)
{
    return hash1.hash[0] == hash2.hash[0]
        && hash1.hash[1] == hash2.hash[1]
        && hash1.hash[2] == hash2.hash[2]
        && hash1.hash[3] == hash2.hash[3];
}

}

/*****************************************************************************/
#endif

