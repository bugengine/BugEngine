#include <bugengine/kernel/input/input.hh>

__kernel void phi_swap(u32 index, u32 total, Kernel::segments< i32 > out1,
                       Kernel::segments< i32 > out2)
{
    be_forceuse(total);
    i32 x1 = *out1;
    i32 x2 = *out2;
    for(u32 i = 0; i < index; ++i)
    {
        /* this code generates the following IR:
          %x2.0 = phi i32 [ %5, %entry ], [ %x1.0, %for.cond ], !dbg !99
          %x1.0 = phi i32 [ %2, %entry ], [ %x2.0, %for.cond ], !dbg !99
          here, x2.0 refers to the value of the previous segment, not the value calculated
          the line above.
          https://www.clear.rice.edu/comp512/Lectures/13SSA-2.pdf
          the generated code will need to handle this case
        */

        i32 t = x2;
        x2    = x1;
        x1    = t;
    }
    *out1 = x1;
    *out2 = x2;
}
