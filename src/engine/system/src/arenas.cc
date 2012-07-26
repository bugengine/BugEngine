/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>

namespace BugEngine { namespace Arena
{

minitl::Allocator& task() { return general(); }
minitl::Allocator& filesystem() { return general(); }
minitl::Allocator& resource() { return general(); }

}}
