/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SAMPLES_KERNEL_STORAGE_SCRIPT_HH_
#define BE_SAMPLES_KERNEL_STORAGE_SCRIPT_HH_
/*****************************************************************************/
#include    <world/entitystorage.factory.hh>
#include    <scheduler/kernel/stream.hh>
#include    <scheduler/kernel/product.hh>
#include    <components.script.hh>

namespace BugEngine
{

class KernelStorage : public World::EntityStorageFactory<
    COMPONENT_LIST_5((A, 12), (B, 16), (C, 12), (D, 32), (E, 3)),
    BugEngine::World::MakePartitionList<
        BugEngine::World::MakePartition<A, B>::Result,
        BugEngine::World::MakePartition<A, C>::Result,
        BugEngine::World::MakePartition<A, D, E>::Result >::Result
    >
{
published:
    KernelStorage();
    ~KernelStorage();
};

}

/*****************************************************************************/
#endif
