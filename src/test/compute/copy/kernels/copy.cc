#include <bugengine/kernel/input/input.hh>
#include <component.script.hh>

__device void kmain(u32 index, u32 total,
                    Kernel::segments< BugEngine::Test::Compute::Copy::CopyComponent > inout)
{
    be_forceuse(index);
    be_forceuse(total);
    be_forceuse(inout);
}
