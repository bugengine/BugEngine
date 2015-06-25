/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SAMPLES_KERNEL_STORAGE_SCRIPT_HH_
#define BE_SAMPLES_KERNEL_STORAGE_SCRIPT_HH_
/**************************************************************************************************/
#include    <stdafx.h>
#include    <world/entitystorage.factory.hh>
#include    <scheduler/kernel/stream.factory.hh>
#include    <scheduler/kernel/product.factory.hh>
#include    <components.script.hh>

namespace BugEngine
{

class KernelStorage : public World::EntityStorageFactory<
    COMPONENT_LIST_7(
        (U1, World::StorageSize_256k),
        (U2, World::StorageSize_64k),
        (A, World::StorageSize_16k),
        (B, World::StorageSize_4k),
        (C, World::StorageSize_4k),
        (D, World::StorageSize_4k),
        (E, World::StorageSize_4k)),
    BugEngine::World::MakePartitionList<
        BugEngine::World::MakePartition<U1, U2>::Result,
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

/**************************************************************************************************/
#endif
