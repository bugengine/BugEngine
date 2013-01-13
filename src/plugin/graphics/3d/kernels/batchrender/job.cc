/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#define be_api(X)
#include    <kernel/compilers.hh>
#include    <kernel/simd.hh>
#include    <kernel/input.hh>
#include    <plugin/dynobjectlist.hh>
using namespace Kernel;

_BE_PLUGIN_EXPORT void kmain(in<i32> input, inout<i32> output)
{
    while(input)
    {
        *output = 2 * *input;
        ++input;
        ++output;
    }
}

