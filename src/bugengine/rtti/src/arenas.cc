/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/rtti/stdafx.h>

namespace BugEngine { namespace Arena {

minitl::Allocator& rtti()
{
    return general();
}
minitl::Allocator& script()
{
    return general();
}

}}  // namespace BugEngine::Arena
