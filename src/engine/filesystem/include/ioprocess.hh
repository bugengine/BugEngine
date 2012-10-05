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
    Semaphore                               m_lock;
    i_u8                                    m_ioDone;
    Thread                                  m_ioThread;
    minitl::intrusive_list<File::Ticket>    m_tickets;
    minitl::istack<File::Ticket>            m_requests;

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
