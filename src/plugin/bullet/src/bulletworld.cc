/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include   <stdafx.h>
#include   <bulletworld.hh>

namespace BugEngine
{

static Allocator& bulletArena() { return gameArena(); }

}

namespace BugEngine { namespace Physics { namespace Bullet
{

static BE_NOINLINE void* allocate(size_t size)
{
    return bulletArena().alloc(size, 16);
}

static BE_NOINLINE void* allocate(size_t size, int align)
{
    return bulletArena().alloc(size, align);
}

static BE_NOINLINE void free(void* block)
{
    return bulletArena().free(block);
}

BulletWorld::WorldSetup::WorldSetup()
{
    btAlignedAllocSetCustom(allocate, free);
    btAlignedAllocSetCustomAligned(allocate, free);
}

BulletWorld::WorldSetup::~WorldSetup()
{
}


BulletWorld::BulletWorld(float3 worldExtents)
:   m_setup()
,   m_configuration()
,   m_dispatcher(&m_configuration)
,   m_solver()
,   m_broadphase(btVector3(-worldExtents.x(),-worldExtents.y(),-worldExtents.z()),btVector3(worldExtents.x(),worldExtents.y(),worldExtents.z()))
,   m_world(&m_dispatcher, &m_broadphase, &m_solver, &m_configuration)
{
}

BulletWorld::~BulletWorld()
{
}

void BulletWorld::step()
{
    m_world.stepSimulation(0.033f);
}

}}}

