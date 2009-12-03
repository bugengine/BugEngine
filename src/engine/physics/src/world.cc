/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <physics/stdafx.h>
#include    <physics/world.hh>

#ifdef BE_COMPILER_MSVC
#pragma warning(push,1)
#endif
#include    <btBulletDynamicsCommon.h>
#include    <LinearMath/btAlignedAllocator.h>
#ifdef BE_COMPILER_MSVC
#pragma warning(pop)
#endif

namespace BugEngine { namespace Physics
{

be_abstractmetaclass_impl("Physics",World);

class World::WorldImplementation
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
    WorldImplementation(float3 worldExtents);
    ~WorldImplementation();
private:
    WorldImplementation(const WorldImplementation& other);
    WorldImplementation& operator=(const WorldImplementation& other);
};

static BE_NOINLINE void* allocate(size_t size)
{
    return Malloc::internalAlloc(size, 16, 3);
}

static BE_NOINLINE void* allocate(size_t size, int align)
{
    return Malloc::internalAlloc(size, align, 3);
}

static BE_NOINLINE void free(void* block)
{
    return Malloc::internalFree(block, 3);
}

World::WorldImplementation::WorldSetup::WorldSetup()
{
    btAlignedAllocSetCustom(allocate, free);
    btAlignedAllocSetCustomAligned(allocate, free);
}

World::WorldImplementation::WorldSetup::~WorldSetup()
{
}


World::WorldImplementation::WorldImplementation(float3 worldExtents)
:   m_setup()
,   m_configuration()
,   m_dispatcher(&m_configuration)
,   m_solver()
,   m_broadphase(btVector3(-worldExtents.x(),-worldExtents.y(),-worldExtents.z()),btVector3(worldExtents.x(),worldExtents.y(),worldExtents.z()))
,   m_world(&m_dispatcher, &m_broadphase, &m_solver, &m_configuration)
{
}

World::WorldImplementation::~WorldImplementation()
{
}

World::World(float3 worldExtents)
:   m_implementation(scoped<WorldImplementation>::create(worldExtents))
{
}

World::~World()
{
}

void World::step(float time)
{
    m_implementation->m_world.stepSimulation(time);
}

}}
