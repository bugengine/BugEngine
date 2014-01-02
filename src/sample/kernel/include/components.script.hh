/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
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

    void created()
    {
        be_info("created A(%d)" | value);
    }
    void destroyed()
    {
        be_info("destroyed A(%d)" | value);
    }
};

be_pod B : public World::Component
{
    u32 value;

    void created()
    {
        be_info("created B(%d)" | value);
    }
    void destroyed()
    {
        be_info("destroyed B(%d)" | value);
    }
};

be_pod C : public World::Component
{
    u32 value;

    void created()
    {
        be_info("created C(%d)" | value);
    }
    void destroyed()
    {
        be_info("destroyed C(%d)" | value);
    }
};

be_pod D : public World::Component
{
    u32 value;

    void created()
    {
        be_info("created D(%d)" | value);
    }
    void destroyed()
    {
        be_info("destroyed D(%d)" | value);
    }
};

be_pod E : public World::Component
{
    u32 value;

    void created()
    {
        be_info("created E(%d)" | value);
    }
    void destroyed()
    {
        be_info("destroyed E(%d)" | value);
    }
};


}

/*****************************************************************************/
#endif
