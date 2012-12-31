/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

/* BEGIN BOILERPLATE */
#include    <kernel/compilers.hh>
#include    <kernel/simd.hh>
#include    <kernel/input.hh>
using namespace Kernel;
#define be_tag(x)
/* END BOILERPLATE */

be_tag(Join(input, output))
void kmain(u32 index, const u32 total, in<i32> input, inout<i32> output)
{
    u32 first = index * input.size() / total;
    u32 last = (index+1) * input.size() / total;
    input += first;
    while(first < last)
    {
        *output += 2 * *input;
        ++input;
        ++output;
        ++first;
    }
}


/* BEGIN BOILERPLATE */
struct Parameter
{
    void* begin;
    void* end;
};

extern "C" BE_EXPORT void _kmain(const u32 index, const u32 total, Parameter argv[])
{
    kmain(index, total, in<i32>((i32*)argv[0].begin, (i32*)argv[0].end), inout<i32>((i32*)argv[1].begin, (i32*)argv[1].end));
}
/* END BOILERPLATE */
