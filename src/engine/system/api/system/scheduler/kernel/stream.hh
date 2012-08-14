/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_SCHEDULER_KERNEL_STREAM_HH_
#define BE_SYSTEM_SCHEDULER_KERNEL_STREAM_HH_
/*****************************************************************************/
#include    <system/scheduler/kernel/istream.hh>

namespace BugEngine { namespace Kernel
{

template< typename T >
class Stream : public IStream
{
};

}}

#define be_stream(x)

/*****************************************************************************/
#endif
