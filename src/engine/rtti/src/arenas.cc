/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>

namespace BugEngine { namespace Arena
{

Allocator& rtti() { return general(); }
Allocator& script() { return general(); }

}}
