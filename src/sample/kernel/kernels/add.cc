/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <components.script.hh>
#include    <cstdio>

void kmain(const u32 /*index*/, const u32 /*total*/,
           segment<const BugEngine::A> input,
           segment<BugEngine::B> output,
           segment<BugEngine::C> output2,
           segment<BugEngine::D> output3,
           segment<BugEngine::E> output4)
{
    while(input)
    {
        printf("%d / %d / %d / %d / %d",
               input->value,
               output->value,
               output2->value,
               output3->value,
               output4->value);
        ++input;
        ++output;
        ++output2;
        ++output3;
        ++output4;
    }
}
