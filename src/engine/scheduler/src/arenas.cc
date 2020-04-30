/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/scheduler/stdafx.h>

namespace BugEngine { namespace Arena {

minitl::Allocator& task()
{
    return general();
}

}}  // namespace BugEngine::Arena
