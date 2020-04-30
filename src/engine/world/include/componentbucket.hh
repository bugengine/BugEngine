/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_WORLD_COMPONENTBUCKET_HH_
#define BE_WORLD_COMPONENTBUCKET_HH_
/**************************************************************************************************/
#include <bugengine/world/stdafx.h>

namespace BugEngine { namespace World {

struct Bucket
{
    u32 acceptMask;
    u32 maskSize;
    u32 componentSize;
    u32 entityCount;
    u32 firstComponent;

    Bucket();
    Bucket(u32 acceptMask, u32 componentSize);
};

}}  // namespace BugEngine::World

/**************************************************************************************************/
#endif
