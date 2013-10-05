/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <runtime/stdafx.h>

#include    <runtime/callstack.hh>

namespace BugEngine { namespace Runtime
{

BE_NOINLINE size_t Callstack::backtrace(Address* /*buffer*/, size_t /*count*/, size_t /*skip*/)
{
    be_warning("backtrace not implemented for MIPS");
    return 0;
}

}}
