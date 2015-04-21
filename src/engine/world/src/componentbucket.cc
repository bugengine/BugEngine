/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <world/stdafx.h>
#include    <componentbucket.hh>

namespace BugEngine { namespace World
{

Bucket::Bucket()
    :   componentCounts()
    ,   acceptMask()
    ,   maskSize()
    ,   firstComponent()
    ,   componentSize()
{
}

Bucket::Bucket(u32* componentCounts, u32 acceptMask, u32 componentSize)
    :   componentCounts(componentCounts)
    ,   acceptMask(acceptMask)
    ,   maskSize(bitCount(acceptMask))
    ,   firstComponent(getFirstBit(acceptMask))
    ,   componentSize(componentSize)
{
}

}}
