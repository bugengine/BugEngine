/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_INTERLOCKED_STACK_HH_
#define BE_MINITL_INTERLOCKED_STACK_HH_
/*****************************************************************************/
#include    <kernel/interlocked.hh>

namespace minitl
{

template< typename T >
class istack
{
public:
    struct inode
    {
        inode* next;
    };
private:
    itaggedptr<inode> m_head;
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
    itaggedptr<inode>::ticket_t ticket;
    do
    {
        ticket = m_head.getTicket();
        t->next = static_cast<T*>((inode*)m_head);
    } while (!m_head.setConditional(t, ticket));
}

template< typename T >
T* istack<T>::pop()
{
    itaggedptr<inode>::ticket_t ticket;
    T* result;
    do
    {
        ticket = m_head.getTicket();
        result = static_cast<T*>((inode*)m_head);
    }
    while (result && !m_head.setConditional(const_cast<inode*>(result->next), ticket));
    return result;
}

}

/*****************************************************************************/
#endif
