/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <world/stdafx.h>
#include    <componentbucket.hh>

namespace BugEngine { namespace World
{

Bucket::Bucket()
    :   componentCounts(0)
    ,   acceptMask(0)
    ,   maskSize(0)
    ,   firstComponent(0)
{
}

Bucket::Bucket(u32* componentCounts, u32 acceptMask)
    :   componentCounts(componentCounts)
    ,   acceptMask(acceptMask)
    ,   maskSize(bitCount(acceptMask))
    ,   firstComponent(getFirstBit(acceptMask))
{
}

}}
