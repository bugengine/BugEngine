#include <bugengine/minitl/stdafx.h>
#include <bugengine/kernel/input/segments.hh>

__global int   x;
__constant int y = 2;

__kernel void add(u32 index, u32 total, Kernel::segments< u32 > p)
{
    while(p)
    {
        *p = x + y;
        p++;
    }
}