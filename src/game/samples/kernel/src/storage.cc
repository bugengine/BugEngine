/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <storage.script.hh>

namespace BugEngine
{

KernelStorage::KernelStorage()
    :   EntityStorage()
    ,   m_stream1(scoped< BugEngine::Kernel::Stream<u32> >::create(Arena::game()))
    ,   m_stream2(scoped< BugEngine::Kernel::Stream<u32> >::create(Arena::game()))
    ,   components1(m_stream1, initialTask())
    ,   components2(m_stream2, initialTask())
{
}

KernelStorage::~KernelStorage()
{
}

}
