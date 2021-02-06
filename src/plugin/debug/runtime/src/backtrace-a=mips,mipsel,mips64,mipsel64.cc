/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.debug.runtime/stdafx.h>
#include <bugengine/plugin.debug.runtime/callstack.hh>

namespace BugEngine { namespace Runtime {

BE_NOINLINE size_t Callstack::backtrace(Address* /*buffer*/, size_t /*count*/, size_t /*skip*/)
{
    be_warning("backtrace not implemented for MIPS");
    return 0;
}

}}  // namespace BugEngine::Runtime
