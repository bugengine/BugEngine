/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_FILESYSTEM_IOPROCESS_HH_
#define BE_FILESYSTEM_IOPROCESS_HH_
/**************************************************************************************************/
#include <bugengine/filesystem/stdafx.h>
#include <bugengine/core/threads/semaphore.hh>
#include <bugengine/core/threads/thread.hh>
#include <bugengine/filesystem/file.script.hh>

namespace BugEngine { namespace IOProcess {

class IOContext : public minitl::pointer
{
private:
    enum
    {
        SlotCount = 32
    };
    Semaphore           m_availableTickets;
    Semaphore           m_freeSlots;
    i_u32               m_firstFreeSlot;
    i_u32               m_firstUsedSlot;
    i_u8                m_ioDone;
    ref< File::Ticket > m_ticketPool[SlotCount];
    Thread              m_ioThread;

private:
    static intptr_t ioProcess(intptr_t p1, intptr_t p2);

public:
    IOContext();
    ~IOContext();

    static void pushTicket(ref< File::Ticket > ticket);
    static void begin();
    static void end();
};

}}  // namespace BugEngine::IOProcess

/**************************************************************************************************/
#endif
