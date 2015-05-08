/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <world/stdafx.h>
#include    <componentbucket.hh>

namespace BugEngine { namespace World
{

Bucket::Bucket()
    :   acceptMask()
    ,   maskSize()
    ,   componentSize()
    ,   entityCount()
    ,   firstComponent()
{
}

Bucket::Bucket(u32 acceptMask, u32 componentSize)
    :   acceptMask(acceptMask)
    ,   maskSize(bitCount(acceptMask))
    ,   componentSize(componentSize)
    ,   entityCount()
    ,   firstComponent(getFirstBit(acceptMask))
{
}

}}
