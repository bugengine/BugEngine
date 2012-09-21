/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

/* BEGIN BOILERPLATE */
#include    <kernel/compilers.hh>
#include    <kernel/simd.hh>
#include    <kernel/input.hh>
using namespace Kernel;
/* END BOILERPLATE */

void kmain(in<i32> input, inout<i32> output)
{
    while(input)
    {
        *output = 2 * *input;
        ++input;
        ++output;
    }
}


/* BEGIN BOILERPLATE */
struct Parameter
{
    void* begin;
    void* end;
};

extern "C" BE_EXPORT void _kmain(Parameter argv[])
{
    kmain(in<i32>((i32*)argv[0].begin, (i32*)argv[0].end), inout<i32>((i32*)argv[1].begin, (i32*)argv[1].end));
}
/* END BOILERPLATE */
