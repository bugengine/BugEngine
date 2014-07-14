/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_FILESYSTEM_IOPROCESS_HH_
#define BE_FILESYSTEM_IOPROCESS_HH_
/**************************************************************************************************/
#include    <filesystem/stdafx.h>
#include    <filesystem/file.script.hh>
#include    <core/threads/thread.hh>
#include    <core/threads/semaphore.hh>

namespace BugEngine { namespace IOProcess
{

class IOContext : public minitl::pointer
{
private:
    enum { SlotCount = 32 };
    Semaphore           m_availableTickets;
    Semaphore           m_freeSlots;
    i_u32               m_firstFreeSlot;
    i_u32               m_firstUsedSlot;
    i_u8                m_ioDone;
    ref<File::Ticket>   m_ticketPool[SlotCount];
    Thread              m_ioThread;
private:
    static intptr_t ioProcess(intptr_t p1, intptr_t p2);

public:
    IOContext();
    ~IOContext();

    static void pushTicket(ref<File::Ticket> ticket);
    static void begin();
    static void end();
};

}}

/**************************************************************************************************/
#endif
