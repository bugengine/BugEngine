/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/resource/stdafx.h>

namespace BugEngine { namespace Arena {

minitl::Allocator& resource()
{
    return general();
}

}}  // namespace BugEngine::Arena
