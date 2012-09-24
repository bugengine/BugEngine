/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_THREADS_QUEUE_INL_
#define BE_CORE_THREADS_QUEUE_INL_
/*****************************************************************************/

namespace BugEngine
{

template< typename T >
Queue<T>::Queue()
    :   m_counter(0)
    ,   m_head()
    ,   m_tail(&m_head)
    ,   m_exit(i_u8::Zero)
{
}

template< typename T >
Queue<T>::~Queue()
{
    m_exit++;
    m_counter.release(1);
}

template< typename T >
void Queue<T>::push(T* t)
{
    typename itaggedptr<Node>::ticket_t ticket;
    do
    {
        ticket = m_head.next.getTicket();
        t->previous = &m_head;
        t->next = m_head.next;
    } while (!m_head.next.setConditional(t, ticket));
    m_counter.release(1);
}

template< typename T >
T* Queue<T>::pop()
{
    m_counter.wait();
    if (m_exit)
    {
        return 0;
    }
    else
    {
        return 0;
    }
}

}

/*****************************************************************************/
#endif
