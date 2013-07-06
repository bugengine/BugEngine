/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <filesystem/stdafx.h>
#include    <watchpoint.hh>

namespace BugEngine { namespace FileSystem
{

ref<Folder::Watch> WatchPoint::addWatch(weak<DiskFolder> /*folder*/, const BugEngine::ipath& /*path*/)
{
    return ref<Folder::Watch>();
}

}}
