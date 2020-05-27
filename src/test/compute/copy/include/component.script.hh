/* BugEngine <bugengine.devel@gmail.com> under New BSD License
   see LICENSE for detail */

#ifndef BE_TEST_COMPUTE_COPY_COMPONENT_SCRIPT_HH_
#define BE_TEST_COMPUTE_COPY_COMPONENT_SCRIPT_HH_
/**************************************************************************************************/
#include <stdafx.h>
#include <bugengine/world/component.script.hh>

namespace BugEngine { namespace Test { namespace Compute { namespace Copy {

be_pod SourceComponent
{
    u64 data[16];
};

be_pod IntermediateComponent
{
    u64 data[16];
};

be_pod TargetComponent
{
    u64 data[16];
};

}}}}  // namespace BugEngine::Test::Compute::Copy

/**************************************************************************************************/
#endif
