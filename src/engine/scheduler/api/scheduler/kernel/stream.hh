/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_STREAM_HH_
#define BE_SCHEDULER_KERNEL_STREAM_HH_
/*****************************************************************************/
#include    <scheduler/kernel/istream.hh>

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