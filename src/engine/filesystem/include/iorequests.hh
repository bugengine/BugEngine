/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
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
