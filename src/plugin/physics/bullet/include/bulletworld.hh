/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_PHYSICSBULLET_BULLETWORLD_H_
#define BE_PHYSICSBULLET_BULLETWORLD_H_
/**************************************************************************************************/
#include <stdafx.h>
#include <bugengine/plugin/plugin.hh>

#ifdef BE_COMPILER_MSVC
#    pragma warning(push, 1)
#endif
#include <LinearMath/btAlignedAllocator.h>
#include <btBulletDynamicsCommon.h>
#ifdef BE_COMPILER_MSVC
#    pragma warning(pop)
#endif

namespace BugEngine { namespace Physics { namespace Bullet {

class BulletWorld : public minitl::refcountable
{
public:
    BulletWorld(const Plugin::Context& context);
    ~BulletWorld();

    void step();

public:
    void* operator new(size_t size, void* where)
    {
        return ::operator new(size, where);
    }
    void operator delete(void* memory, void* where)
    {
        ::operator delete(memory, where);
    }
    void operator delete(void* memory)
    {
        be_notreached();
        ::operator delete(memory);
    }
};

}}}  // namespace BugEngine::Physics::Bullet

/**************************************************************************************************/
#endif
