/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SAMPLES_KERNEL_STORAGE_SCRIPT_HH_
#define BE_SAMPLES_KERNEL_STORAGE_SCRIPT_HH_
/*****************************************************************************/
#include    <world/entitystorage.script.hh>

namespace BugEngine
{

class KernelStorage : public World::EntityStorage
{
private:
    scoped< BugEngine::Kernel::Stream<u32> >        m_stream1;
    scoped< BugEngine::Kernel::Stream<u32> >        m_stream2;
published:
    weak< BugEngine::Kernel::Stream<u32> > const    stream1;
    weak< BugEngine::Kernel::Stream<u32> > const    stream2;
published:
    KernelStorage();
    ~KernelStorage();
};

}

/*****************************************************************************/
#endif
