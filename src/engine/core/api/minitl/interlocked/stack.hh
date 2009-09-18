/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_INTERLOCKED_STACK_
#define BE_MINITL_INTERLOCKED_STACK_
/*****************************************************************************/

namespace minitl
{

struct inode
{
    iptr<inode> next;
};

template< typename T >
class istack
{
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
:   m_head()
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
    } while(!m_head.setConditional(t, ticket));
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
    while(!m_head.setConditional((inode*)result->next, ticket));
    return result;
}

}

/*****************************************************************************/
#endif
