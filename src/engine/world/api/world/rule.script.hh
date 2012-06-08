/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_WORLD_RULE_SCRIPT_HH_
#define BE_WORLD_RULE_SCRIPT_HH_
/*****************************************************************************/
#include    <world/entity.script.hh>
#include    <world/component.script.hh>


namespace BugEngine { namespace World
{
/*
struct A : Component
{
    i32 r;
    void created() { r = 2; }
};
struct B : Component
{
    i32 r;
    void created() { r = 4; }
};
struct C : Component
{
    i32 r;
    void created() { r = 0; }
};

static inline void work(in<A> a, in<B> b, out<C> c)
{
    d.r += a.r + b.r;
}

class Rule
{
    void work(const A* aBegin, const A* aEnd, const B* bBegin, const B* bEnd, C* cBegin, C* cEnd)
    {
        be_assert(aBegin - aEnd == bBegin - bEnd, "got mismatching argument count");
        be_assert(aBegin - aEnd == cBegin - cEnd, "got mismatching argument count");
        for(; aBegin != aEnd; aBegin++, bBegin++, cBegin++)
            work(aBegin, bBegin, cBegin);
    }
};
*/

}}


/*****************************************************************************/
#endif
