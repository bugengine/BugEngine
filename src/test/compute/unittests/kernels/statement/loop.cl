#include <bugengine/kernel/input/segments.hh>
#include <component.script.hh>

using namespace BugEngine::Test::Compute::UnitTests;

__kernel void for_loop(u32 index, u32 total, Kernel::segments< ComponentInt > inout)
{
    Kernel::segments< ComponentInt >::iterator first
        = inout.begin() + (index * inout.size() / total);
    Kernel::segments< ComponentInt >::iterator last
        = inout.begin() + ((index + 1) * inout.size() / total);
    for(Kernel::segments< ComponentInt >::iterator it = first; it != last; ++it)
    {
        it->value *= 2;
    }
}

__kernel void while_loop(u32 index, u32 total, Kernel::segments< ComponentInt > inout)
{
    Kernel::segments< ComponentInt >::iterator first
        = inout.begin() + (index * inout.size() / total);
    Kernel::segments< ComponentInt >::iterator last
        = inout.begin() + ((index + 1) * inout.size() / total);
    while(first != last)
    {
        (first++)->value *= 3;
    }
}
