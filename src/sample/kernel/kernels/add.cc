/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include <components.script.hh>

void kmain(u32 index, const u32 total,
           in<BugEngine::A> input,
           inout<BugEngine::B> output,
           inout<BugEngine::C> output2,
           inout<BugEngine::D> output3)
{
    u32 first = index * input.size() / total;
    u32 last = (index+1) * input.size() / total;
    input += first;
    while(first < last)
    {
        output->value += 2 * input->value;
        output2->value += 3 * input->value;
        output3->value += 4 * input->value;
        ++input;
        ++output;
        ++output2;
        ++output3;
        ++first;
    }
}
