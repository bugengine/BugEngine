/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/world/stdafx.h>

namespace BugEngine { namespace Arena {

minitl::Allocator& game()
{
    return general();
}

}}  // namespace BugEngine::Arena
