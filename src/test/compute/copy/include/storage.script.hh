/* BugEngine <bugengine.devel@gmail.com> under New BSD License
   see LICENSE for detail */

#ifndef BE_TEST_COMPUTE_COPY_STORAGE_SCRIPT_H_
#define BE_TEST_COMPUTE_COPY_STORAGE_SCRIPT_H_
/**************************************************************************************************/
#include <bugengine/world/entitystorage.factory.hh>
#include <component.script.hh>

namespace BugEngine { namespace Test { namespace Compute { namespace Copy {

class Storage
    : public World::EntityStorageFactory<
         COMPONENT_LIST_1((CopyComponent, World::StorageSize_256k)),
         World::MakePartitionList<
            BugEngine::World::MakePartition< CopyComponent >::Result >::Result >
{
    published : Storage();
    ~Storage();
};

}}}}  // namespace BugEngine::Test::Compute::Copy

/**************************************************************************************************/
#endif
