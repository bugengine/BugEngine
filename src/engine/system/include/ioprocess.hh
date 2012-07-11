/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_IOPROCESS_HH_
#define BE_SYSTEM_IOPROCESS_HH_
/*****************************************************************************/
#include    <system/file/file.script.hh>

namespace BugEngine { namespace IOProcess
{

struct IOContext;
be_api(SYSTEM) void pushTicket(ref<File::Ticket> ticket);

}}

/*****************************************************************************/
#endif
