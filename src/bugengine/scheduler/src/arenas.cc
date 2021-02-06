/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/scheduler/stdafx.h>

namespace BugEngine { namespace Arena {

minitl::Allocator& task()
{
    return general();
}

}}  // namespace BugEngine::Arena
