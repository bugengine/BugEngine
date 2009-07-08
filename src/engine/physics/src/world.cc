/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

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

be_metaclass_impl("Physics",World);

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
    WorldSetup                                  m_setup;
    scopedptr<btDefaultCollisionConfiguration>  m_configuration;
    scopedptr<btDispatcher>                     m_dispatcher;
    scopedptr<btConstraintSolver>               m_solver;
    scopedptr<btBroadphaseInterface>            m_broadphase;
    scopedptr<btDynamicsWorld>                  m_world;
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
,   m_configuration(new btDefaultCollisionConfiguration)
,   m_dispatcher(new btCollisionDispatcher(m_configuration.get()))
,   m_broadphase(new btAxisSweep3(btVector3(-worldExtents.x(),-worldExtents.y(),-worldExtents.z()),btVector3(worldExtents.x(),worldExtents.y(),worldExtents.z())))
,   m_solver(new btSequentialImpulseConstraintSolver)
,   m_world(new btDiscreteDynamicsWorld(m_dispatcher.get(),m_broadphase.get(),m_solver.get(),m_configuration.get()))
{
}

World::WorldImplementation::~WorldImplementation()
{
}

World::World(float3 worldExtents)
:   m_implementation(new WorldImplementation(worldExtents))
{
}

World::~World()
{
}

void World::step(float time)
{
    m_implementation->m_world->stepSimulation(time);
}

}}
