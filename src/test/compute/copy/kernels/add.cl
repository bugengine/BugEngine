#include <bugengine/minitl/stdafx.h>
#include <bugengine/kernel/input/segments.hh>

__global int x;

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
    __local int y;
    be_forceuse(index);
    be_forceuse(total);
    while(p)
    {
        *p = *findMin(&x, &y);
        p++;
    }
}