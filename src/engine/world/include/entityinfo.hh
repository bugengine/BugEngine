/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_WORLD_ENTITYINFO_HH_
#define BE_WORLD_ENTITYINFO_HH_
/**************************************************************************************************/
#include    <world/stdafx.h>
#include    <minitl/bitset.hh>

namespace BugEngine { namespace World
{

struct EntityInfo
{
    union
    {
        u32 next;
        u32 index;
    };
    minitl::bitset<96>  mask;
    u32 componentIndex[1];
};

static const u32 s_usedBit = 0x80000000;
static const u32 s_indexMask = 0x7FFFFFFF;

}}

/**************************************************************************************************/
#endif
