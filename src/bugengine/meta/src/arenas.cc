/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/meta/stdafx.h>

namespace BugEngine { namespace Arena {

minitl::Allocator& meta()
{
    return general();
}
minitl::Allocator& script()
{
    return general();
}

}}  // namespace BugEngine::Arena
