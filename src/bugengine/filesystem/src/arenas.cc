/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/filesystem/stdafx.h>

namespace BugEngine { namespace Arena {

minitl::Allocator& filesystem()
{
    return general();
}

}}  // namespace BugEngine::Arena
