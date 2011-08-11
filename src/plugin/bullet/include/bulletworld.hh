/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_PHYSICSBULLET_BULLETWORLD_H_
#define BE_PHYSICSBULLET_BULLETWORLD_H_
/*****************************************************************************/

#ifdef BE_COMPILER_MSVC
#pragma warning(push,1)
#endif
#include    <btBulletDynamicsCommon.h>
#include    <LinearMath/btAlignedAllocator.h>
#ifdef BE_COMPILER_MSVC
#pragma warning(pop)
#endif

namespace BugEngine { namespace Physics { namespace Bullet
{

class BulletWorld : public minitl::pointer
{
private:
    class WorldSetup
    {
    public:
        WorldSetup();
        ~WorldSetup();
    };
public:
    WorldSetup                          m_setup;
    btDefaultCollisionConfiguration     m_configuration;
    btCollisionDispatcher               m_dispatcher;
    btSequentialImpulseConstraintSolver m_solver;
    btAxisSweep3                        m_broadphase;
    btDiscreteDynamicsWorld             m_world;
public:
    BulletWorld(float3 worldExtents);
    ~BulletWorld();

    void step();

public:
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
    void  operator delete(void* memory, void* where) { return ::operator delete(memory, where); }
    void  operator delete(void* memory)              { be_notreached(); return ::operator delete(memory); }
};

}}}

/*****************************************************************************/
#endif

