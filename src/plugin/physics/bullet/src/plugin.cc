/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <stdafx.h>
#include    <bulletworld.hh>
#include    <plugin/plugin.hh>

static ref<BugEngine::Physics::Bullet::BulletWorld> create(const BugEngine::Plugin::Context& context)
{
    return ref<BugEngine::Physics::Bullet::BulletWorld>::create(BugEngine::Arena::game(), context);
}

BE_PLUGIN_REGISTER(BugEngine::Physics::Bullet::BulletWorld, &create);
