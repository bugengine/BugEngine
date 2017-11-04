/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SAMPLES_PARTICLERAIN_STORAGE_SCRIPT_HH_
#define BE_SAMPLES_PARTICLERAIN_STORAGE_SCRIPT_HH_
/**************************************************************************************************/
#include    <stdafx.h>
#include    <world/entitystorage.factory.hh>
#include    <components/obstacle.script.hh>
#include    <components/particle.script.hh>

namespace BugEngine { namespace ParticleRain
{

class Storage : public World::EntityStorageFactory<
    COMPONENT_LIST_2(
        (Particle, World::StorageSize_256k),
        (Obstacle, World::StorageSize_4k)),
    BugEngine::World::MakePartitionList< BugEngine::World::MakePartition<Particle>::Result,
                                         BugEngine::World::MakePartition<Obstacle>::Result>::Result
    >
{
published:
    Storage();
    ~Storage();
};

}}

/**************************************************************************************************/
#endif
