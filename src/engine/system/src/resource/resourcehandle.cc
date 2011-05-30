/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/resource/resourcehandle.hh>
#include    <system/resource/resourceloader.hh>

namespace BugEngine
{

static ResourceHandle::Id defaultId() { ResourceHandle::Id id =  { 0 }; return id; }
ResourceHandle::ResourceHandle()
    :   handle()
    ,   id(defaultId())
{
}

ResourceHandle::~ResourceHandle()
{
    be_assert(id.intId == 0, "resource handle destroyed but hasn't been properly unloaded");
}

const ResourceHandle& ResourceHandle::null()
{
    static ResourceHandle s_nullHandle;
    return s_nullHandle;
}

}
