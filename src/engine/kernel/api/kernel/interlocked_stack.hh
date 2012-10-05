/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_KERNEL_INTERLOCKED_STACK_HH_
#define BE_KERNEL_INTERLOCKED_STACK_HH_
/*****************************************************************************/
#include    <kernel/interlocked.hh>

namespace minitl
{

template< typename T >
class istack
{
public:
    struct node
    {
        node* next;
    };
private:
    itaggedptr<node> m_head;
public:
    istack();
    ~istack();

    void    push(T* t);
    T*      pop();
};

template< typename T >
istack<T>::istack()
:   m_head(0)
{
}

template< typename T>
istack<T>::~istack()
{
}

template< typename T >
void istack<T>::push(T* t)
{
    typename itaggedptr<node>::ticket_t ticket;
    do
    {
        ticket = m_head.getTicket();
        t->next = static_cast<T*>((node*)m_head);
    } while (!m_head.setConditional(t, ticket));
}

template< typename T >
T* istack<T>::pop()
{
    typename itaggedptr<node>::ticket_t ticket;
    T* result;
    do
    {
        ticket = m_head.getTicket();
        result = static_cast<T*>((node*)m_head);
    }
    while (result && !m_head.setConditional(const_cast<node*>(result->next), ticket));
    return result;
}

}

/*****************************************************************************/
#endif
