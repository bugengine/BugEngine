/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <filesystem/stdafx.h>
#include    <watchpoint.hh>

namespace BugEngine { namespace FileSystem
{

ref<Folder::Watch> WatchPoint::addWatch(weak<DiskFolder> /*folder*/,
                                        const BugEngine::ipath& /*path*/)
{
    return ref<Folder::Watch>();
}

}}
