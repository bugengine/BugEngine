/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>

namespace BugEngine { namespace Arena
{

minitl::Allocator& rtti() { return general(); }
minitl::Allocator& script() { return general(); }

}}
