#include <bugengine/minitl/stdafx.h>
#include <bugengine/kernel/input/segments.hh>

static kernel_global int x;

__device int* findMin(int* x1, int* /*x2*/)
{
    /*if(*x1 < *x2)
        return x1;
    else
        return x2;*/
    return x1;
}

__kernel void add(u32 index, u32 total, Kernel::segments< u32 > p)
{
    kernel_local int y;
    be_forceuse(index);
    be_forceuse(total);
    for(Kernel::segments< u32 >::iterator it = p.begin(); it != p.end(); ++it)
    {
        *it = *findMin(&x, &y);
    }
}
