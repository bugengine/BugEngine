/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/filesystem/stdafx.h>
#include <ioprocess.hh>

namespace BugEngine { namespace IOProcess {

scoped< IOContext > s_context;
static i_u32        s_contextUseCount;

IOContext::IOContext()
    : m_availableTickets(0)
    , m_freeSlots(SlotCount)
    , m_firstFreeSlot(i_u32::create(0))
    , m_firstUsedSlot(i_u32::create(0))
    , m_ioDone(i_u8::create(0))
    , m_ioThread("IOThread", &IOContext::ioProcess, reinterpret_cast< intptr_t >(this), 0,
                 Thread::Highest)
{
}

IOContext::~IOContext()
{
    m_ioDone++;
    m_availableTickets.release(1);
    m_ioThread.wait();
    for(int i = 0; i < SlotCount; ++i)
    {
        m_ticketPool[i] = ref< File::Ticket >();
    }
}

intptr_t IOContext::ioProcess(intptr_t p1, intptr_t /*p2*/)
{
    IOContext* context = reinterpret_cast< IOContext* >(p1);
    while(1)
    {
        context->m_availableTickets.wait();
        u32                 slotIndex = context->m_firstUsedSlot % SlotCount;
        ref< File::Ticket > request   = context->m_ticketPool[slotIndex];
        if(!request)
        {
            be_assert(context->m_ioDone, "IO context exited but was not yet finished");
            break;
        }
        context->m_ticketPool[slotIndex] = ref< File::Ticket >();
        context->m_firstUsedSlot++;
        context->m_freeSlots.release(1);

        switch(request->action)
        {
        case File::Ticket::Read:
            if(!context->m_ioDone)
            {
                if(request->text)
                {
                    request->buffer.realloc(request->total + 1);
                    request->buffer[request->total] = 0;
                }
                else
                {
                    request->buffer.realloc(request->total);
                }
                request->processed = 0;
                request->file->fillBuffer(request);
            }
            break;
        case File::Ticket::Write: request->file->writeBuffer(request); break;
        default: be_error("unknown IO request: %d" | request->action); break;
        }
    }
    return 0;
}

void IOContext::pushTicket(ref< File::Ticket > ticket)
{
    s_context->m_freeSlots.wait();
    u32 slot = s_context->m_firstFreeSlot++;
    be_assert(slot < s_context->m_firstUsedSlot + SlotCount, "circular buffer inconsistency");
    slot                          = slot % SlotCount;
    s_context->m_ticketPool[slot] = ticket;
    s_context->m_availableTickets.release(1);
}

void IOContext::begin()
{
    if(++s_contextUseCount == 1)
    {
        s_context.reset(scoped< IOContext >::create(Arena::filesystem()));
    }
}

void IOContext::end()
{
    if(--s_contextUseCount == 0)
    {
        s_context.reset(scoped< IOContext >());
    }
}

}}  // namespace BugEngine::IOProcess
