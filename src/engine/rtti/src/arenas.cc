/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <rtti/stdafx.h>

namespace BugEngine { namespace Arena
{

minitl::Allocator& rtti() { return general(); }
minitl::Allocator& script() { return general(); }

}}
