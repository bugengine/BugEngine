/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <bulletworldmanager.hh>
#include    <bulletworld.hh>

namespace BugEngine { namespace Physics { namespace Bullet
{

BulletWorldManager::BulletWorldManager()
{
}

BulletWorldManager::~BulletWorldManager()
{
}

ref<IPhysicsWorld> BulletWorldManager::createWorld(float3 extents, weak<BaseTask::Callback> endJob) const
{
    return ref<BulletWorld>::create(extents, endJob);
}

}}}

