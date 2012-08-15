/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_IOREQUESTS_HH_
#define BE_SYSTEM_IOREQUESTS_HH_
/*****************************************************************************/
#include    <filesystem/file.script.hh>

namespace BugEngine { namespace IOProcess
{

void pushTicket(ref<File::Ticket> ticket);

}}

/*****************************************************************************/
#endif
