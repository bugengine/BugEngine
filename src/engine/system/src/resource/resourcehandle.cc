/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/resource/resourcehandle.hh>
#include    <system/resource/iresourceloader.hh>

namespace BugEngine
{

ResourceHandle::ResourceHandle()
    :   owner(0)
    ,   resource(0)
{
}

ResourceHandle::~ResourceHandle()
{
    if (owner)
    {
        owner->unload(resource);
    }
}

}

