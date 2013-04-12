/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

/* BEGIN BOILERPLATE */
#define be_api(X)
#include    <kernel/compilers.hh>
#include    <kernel/simd.hh>
#include    <kernel/input.hh>
#include    <plugin/dynobjectlist.hh>
using namespace Kernel;
#define be_tag(x)
#define be_product(x)
/* END BOILERPLATE */

#include <components.script.hh>

void kmain(u32 index, const u32 total, in<BugEngine::A> input, inout<BugEngine::B> output)
{
    u32 first = index * input.size() / total;
    u32 last = (index+1) * input.size() / total;
    input += first;
    while(first < last)
    {
        output->value += 2 * input->value;
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

_BE_PLUGIN_EXPORT void _kmain(const u32 index, const u32 total, Parameter argv[])
{
    kmain(index, total,
          in<BugEngine::A>((BugEngine::A*)argv[0].begin, (BugEngine::A*)argv[0].end),
          inout<BugEngine::B>((BugEngine::B*)argv[1].begin, (BugEngine::B*)argv[1].end));
}
_BE_REGISTER_PLUGIN(BE_KERNELID, BE_KERNELNAME);
_BE_REGISTER_METHOD(BE_KERNELID, _kmain);

/* END BOILERPLATE */
