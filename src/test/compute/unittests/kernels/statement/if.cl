#include <bugengine/kernel/input/segments.hh>
#include <component.script.hh>

using namespace BugEngine::Test::Compute::UnitTests;

__kernel void if_then(u32 index, u32 total, Kernel::segments< ComponentInt > inout)
{
    Kernel::segments< ComponentInt >::iterator first
        = inout.begin() + (index * inout.size() / total);
    Kernel::segments< ComponentInt >::iterator last
        = inout.begin() + ((index + 1) * inout.size() / total);
    for(Kernel::segments< ComponentInt >::iterator it = first; it != last; ++it)
    {
        if(it->value < 0) it->value = -it->value;
    }
}

__kernel void if_then_else(u32 index, u32 total, Kernel::segments< ComponentInt > inout)
{
    Kernel::segments< ComponentInt >::iterator first
        = inout.begin() + (index * inout.size() / total);
    Kernel::segments< ComponentInt >::iterator last
        = inout.begin() + ((index + 1) * inout.size() / total);
    for(Kernel::segments< ComponentInt >::iterator it = first; it != last; ++it)
    {
        if(it->value % 2 == 0)
            it->value = -it->value;
        else
            it->value = 0;
    }
}
