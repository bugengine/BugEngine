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

class KernelStorage : public World::EntityStorageFactory< COMPONENT_LIST_2(A, B) >
{
published:
    KernelStorage();
    ~KernelStorage();
};

}

/*****************************************************************************/
#endif
