/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>

namespace BugEngine { namespace Arena
{

Allocator& task() { return general(); }
Allocator& filesystem() { return general(); }
Allocator& resource() { return general(); }

}}
