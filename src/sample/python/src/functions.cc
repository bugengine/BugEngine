/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <stdafx.h>
#include <bugengine/meta/engine/namespace.hh>
#include <functions.script.hh>

namespace BugEngine { namespace TestCases {

void Class::doStuff(u32 v1, u32 v2, u32 v3)
{
    be_info("v1: %d | v2: %d | v3: %d" | v1 | v2 | v3);
}

void Class::doStuff(float v1, float v2)
{
    be_info("v1: %f | v2: %f" | v1 | v2);
}

void Class::doStuff(u32 v1, u32 v2, bool done)
{
    be_info("v1: %d | v2: %d | done : %s" | v1 | v2 | done);
}

}}  // namespace BugEngine::TestCases
