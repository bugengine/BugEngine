#include <kernel/input/segments.hh>

__device void kmain(u32 index, u32 total, Kernel::segments<float> a, Kernel::segments<float> b)
{
    be_forceuse(index);
    be_forceuse(total);
    be_forceuse(a);
    be_forceuse(b);
}

