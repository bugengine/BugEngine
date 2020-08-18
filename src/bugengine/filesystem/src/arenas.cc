/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/filesystem/stdafx.h>

namespace BugEngine { namespace Arena {

minitl::Allocator& filesystem()
{
    return general();
}

}}  // namespace BugEngine::Arena
