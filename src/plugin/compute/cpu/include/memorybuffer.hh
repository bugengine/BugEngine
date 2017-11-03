/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_COMPUTE_CPU_MEMORYBUFFER_HH_
#define BE_COMPUTE_CPU_MEMORYBUFFER_HH_
/**************************************************************************************************/
#include    <stdafx.h>
#include    <scheduler/kernel/imemorybuffer.hh>


namespace BugEngine { namespace Kernel { namespace CPU
{

class MemoryHost;

class MemoryBuffer : public IMemoryBuffer
{
public:
    MemoryBuffer(weak<const MemoryHost> provider);
    ~MemoryBuffer();
};

}}}

/**************************************************************************************************/
#endif
