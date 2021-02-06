/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/resource/stdafx.h>

namespace BugEngine { namespace Arena {

minitl::Allocator& resource()
{
    return general();
}

}}  // namespace BugEngine::Arena
