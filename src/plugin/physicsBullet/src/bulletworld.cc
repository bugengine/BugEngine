/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include   <stdafx.h>
#include   <bulletworld.hh>

namespace BugEngine
{

template< >
void* Memory<Arena::Plugin>::systemAlloc(size_t size, size_t alignment)
{
#ifdef _MSC_VER
    return ::_aligned_malloc(size, alignment);
#else
    return ::malloc(size);
#endif
}

template< >
void* Memory<Arena::Plugin>::systemRealloc(void* ptr, size_t size, size_t alignment)
{
#ifdef _MSC_VER
    return ::_aligned_realloc(ptr, size, alignment);
#else
    return ::realloc(ptr, size);
#endif
}

template< >
void Memory<Arena::Plugin>::systemFree(const void* pointer)
{
#ifdef _MSC_VER
    return ::_aligned_free(const_cast<void*>(pointer));
#else
    return ::free(const_cast<void*>(pointer));
#endif
}

}

namespace BugEngine { namespace Physics { namespace Bullet
{

static BE_NOINLINE void* allocate(size_t size)
{
    return Memory<Arena::Plugin>::alloc(size, 16);
}

static BE_NOINLINE void* allocate(size_t size, int align)
{
    return Memory<Arena::Plugin>::alloc(size, align);
}

static BE_NOINLINE void free(void* block)
{
    return Memory<Arena::Plugin>::free(block);
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
:   IWorld()
,   m_setup()
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

