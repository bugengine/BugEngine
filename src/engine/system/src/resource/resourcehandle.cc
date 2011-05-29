/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/resource/resourcehandle.hh>
#include    <system/resource/resourceloader.hh>

namespace BugEngine
{

ResourceHandle::ResourceHandle()
    :   owner(0)
    ,   resource(0)
{
}

ResourceHandle::~ResourceHandle()
{
    be_assert(owner == 0, "resource handle destroyed but hasn't been properly unloaded");
    be_assert(resource == 0, "resource handle destroyed but hasn't been properly unloaded");
}

}

