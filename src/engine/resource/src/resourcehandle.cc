/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <resource/stdafx.h>
#include    <resource/resourcehandle.hh>
#include    <resource/resourceloader.hh>

namespace BugEngine
{

static ResourceHandle::Id defaultId() { ResourceHandle::Id id =  { 0 }; return id; }
ResourceHandle::ResourceHandle()
    :   handle()
    ,   cache(defaultId())
{
}

ResourceHandle::~ResourceHandle()
{
    be_assert(cache.intId == 0, "resource handle destroyed but hasn't been properly unloaded");
}

ResourceHandle& ResourceHandle::null()
{
    static ResourceHandle s_nullHandle;
    return s_nullHandle;
}

}
