/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SAMPLES_KERNEL_COMPONENTS_SCRIPT_HH_
#define BE_SAMPLES_KERNEL_COMPONENTS_SCRIPT_HH_
/*****************************************************************************/
#include    <world/component.script.hh>

namespace BugEngine
{

be_pod U1 : public World::Component
{
    u32 value;
};

be_pod U2 : public World::Component
{
    u32 value;
};

be_pod A : public World::Component
{
    u32 value;

    void created();
    void destroyed();
};

be_pod B : public World::Component
{
    u32 value;

    void created();
    void destroyed();
};

be_pod C : public World::Component
{
    u32 value;

    void created();
    void destroyed();
};

be_pod D : public World::Component
{
    u32 value;

    void created();
    void destroyed();
};

be_pod E : public World::Component
{
    u32 value;

    void created();
    void destroyed();
};


}

/*****************************************************************************/
#endif
