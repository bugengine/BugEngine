#include <bugengine/kernel/input/input.hh>
#include <component.script.hh>

template < typename T1, typename T2 >
__device void copy(u32 index, u32 total, Kernel::segments< T1 >& in, Kernel::segments< T2 >& out)
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

__kernel void copyA(u32 index, u32 total,
                    Kernel::segments< BugEngine::Test::Compute::Copy::SourceComponent >       in,
                    Kernel::segments< BugEngine::Test::Compute::Copy::IntermediateComponent > out)
{
    copy(index, total, in, out);
}

__kernel void copyB(u32 index, u32 total,
                    Kernel::segments< BugEngine::Test::Compute::Copy::IntermediateComponent > in,
                    Kernel::segments< BugEngine::Test::Compute::Copy::TargetComponent >       out)
{
    copy(index, total, in, out);
}
