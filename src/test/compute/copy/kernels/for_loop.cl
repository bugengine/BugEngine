#include <bugengine/kernel/input/input.hh>

__kernel void for_loop_1(u32 index, u32 total, Kernel::segments< i32 > out1,
                         Kernel::segments< i32 > out2)
{
    for(u32 i = index; i < total; ++i)
    {
        for(u32 j = i; j < total; ++j)
        {
            *out2 = *out2 + *out1;
        }
    }
}
