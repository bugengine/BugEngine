/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/filesystem/stdafx.h>
#include <watchpoint.hh>

namespace BugEngine { namespace FileSystem {

ref< Folder::Watch > WatchPoint::addWatch(weak< DiskFolder > /*folder*/,
                                          const BugEngine::ipath& /*path*/)
{
    return ref< Folder::Watch >();
}

}}  // namespace BugEngine::FileSystem
