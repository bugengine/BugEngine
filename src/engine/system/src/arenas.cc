/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>

namespace BugEngine { namespace Arena
{

Allocator& task() { return general(); }
Allocator& filesystem() { return general(); }
Allocator& resource() { return general(); }

}}
