/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>

namespace BugEngine { namespace Arena
{

Allocator& rtti() { return general(); }
Allocator& script() { return general(); }

}}
