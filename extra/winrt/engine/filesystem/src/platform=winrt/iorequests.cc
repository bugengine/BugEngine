/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <filesystem/stdafx.h>
#include    <iorequests.hh>
#include    <ioprocess.hh>

namespace BugEngine { namespace IOProcess
{

static IOContext s_context;

void pushTicket(ref<File::Ticket> ticket)
{
    s_context.pushTicket(ticket);
}

}}
