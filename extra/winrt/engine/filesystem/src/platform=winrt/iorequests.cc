/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
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
