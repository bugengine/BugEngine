/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <bulletworld.hh>
#include    <system/plugin.hh>

BE_PLUGIN_REGISTER(physicsBullet, BugEngine::Physics::Bullet::BulletWorld, (BugEngine::float3 worldExtents), (worldExtents));

