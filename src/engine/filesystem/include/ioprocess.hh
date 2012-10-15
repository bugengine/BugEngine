/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_FILESYSTEM_IOPROCESS_HH_
#define BE_FILESYSTEM_IOPROCESS_HH_
/*****************************************************************************/
#include    <filesystem/file.script.hh>
#include    <core/threads/thread.hh>
#include    <core/threads/semaphore.hh>

namespace BugEngine { namespace IOProcess
{

class IOContext
{
private:
    enum { SlotCount = 128 };
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
    void pushTicket(ref<File::Ticket> ticket);
};

}}

/*****************************************************************************/
#endif
