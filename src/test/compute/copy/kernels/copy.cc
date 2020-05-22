#include <bugengine/kernel/input/input.hh>
#include <component.script.hh>

__kernel void copy(u32 index, u32 total,
                   Kernel::segments< BugEngine::Test::Compute::Copy::SourceComponent > in,
                   Kernel::segments< BugEngine::Test::Compute::Copy::TargetComponent > out)
{
    u32 start = (index * in.size()) / total;
    u32 end   = ((index + 1) * in.size()) / total;
    in += start;
    out += start;

    for(u32 x = start; x < end; ++x)
    {
        for(u32 i = 0; i < sizeof(out->data) / sizeof(out->data[0]); ++i)
            out->data[i] = in->data[i];
        ++out;
        ++in;
    }
}
