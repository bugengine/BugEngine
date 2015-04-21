/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_WORLD_COMPONENTBUCKET_HH_
#define BE_WORLD_COMPONENTBUCKET_HH_
/**************************************************************************************************/
#include    <world/stdafx.h>

namespace BugEngine { namespace World
{

struct Bucket
{
    u32* componentCounts;
    u32 acceptMask;
    u32 maskSize;
    u32 firstComponent;
    u32 componentSize;

    Bucket();
    Bucket(u32* componentCounts, u32 acceptMask, u32 componentSize);
};

}}


/**************************************************************************************************/
#endif
