/* BugEngine <bugengine.devel@gmail.com> under New BSD License
   see LICENSE for detail */

#ifndef BE_TEST_COMPUTE_UNITTESTS_STORAGE_SCRIPT_H_
#define BE_TEST_COMPUTE_UNITTESTS_STORAGE_SCRIPT_H_
/**************************************************************************************************/
#include <bugengine/world/entitystorage.factory.hh>
#include <component.script.hh>

namespace BugEngine { namespace Test { namespace Compute { namespace UnitTests {

typedef BugEngine::World::MakePartition<
    ComponentFloat, ComponentFloat2, ComponentFloat3, ComponentFloat4, ComponentFloat8,
    ComponentFloat16, ComponentInt, ComponentInt2, ComponentInt3, ComponentInt4, ComponentInt8,
    ComponentInt16, ComponentDouble, ComponentDouble2, ComponentDouble3, ComponentDouble4,
    ComponentDouble8, ComponentDouble16 >::Result Partition;

class Storage
    : public World::EntityStorageFactory<
          BugEngine::World::MakeComponentList<
              ComponentFloat, World::StorageSize_256k, ComponentFloat2, World::StorageSize_256k,
              ComponentFloat3, World::StorageSize_256k, ComponentFloat4, World::StorageSize_256k,
              ComponentFloat8, World::StorageSize_256k, ComponentFloat16, World::StorageSize_256k,
              ComponentInt, World::StorageSize_256k, ComponentInt2, World::StorageSize_256k,
              ComponentInt3, World::StorageSize_256k, ComponentInt4, World::StorageSize_256k,
              ComponentInt8, World::StorageSize_256k, ComponentInt16, World::StorageSize_256k,
              ComponentDouble, World::StorageSize_256k, ComponentDouble2, World::StorageSize_256k,
              ComponentDouble3, World::StorageSize_256k, ComponentDouble4, World::StorageSize_256k,
              ComponentDouble8, World::StorageSize_256k, ComponentDouble16,
              World::StorageSize_256k >::Result,
          World::MakePartitionList< Partition >::Result >
{
    published : Storage();
    ~Storage();
};

}}}}  // namespace BugEngine::Test::Compute::UnitTests

/**************************************************************************************************/
#endif
