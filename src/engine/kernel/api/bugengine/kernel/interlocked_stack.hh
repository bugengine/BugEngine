/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_KERNEL_INTERLOCKED_STACK_HH_
#define BE_KERNEL_INTERLOCKED_STACK_HH_
/**************************************************************************************************/
#include <bugengine/kernel/stdafx.h>
#include <bugengine/kernel/interlocked.hh>

namespace minitl {

template < typename T >
struct itaggedptr
{
private:
    typedef _Kernel::InterlockedType< sizeof(T*) > impl;
    typedef typename impl::tagged_t                type_t;
    typedef typename type_t::value_t               value_t;

private:
    type_t m_value;

public:
    itaggedptr(T* t) : m_value((typename type_t::value_t)(t))
    {
    }
    typedef typename type_t::tag_t ticket_t;

    operator const T*() const
    {
        return static_cast< T* >(m_value.value());
    }
    operator T*()
    {
        return static_cast< T* >(m_value.value());
    }
    T* operator->()
    {
        return static_cast< T* >(m_value.value());
    }
    const T* operator->() const
    {
        return static_cast< T* >(m_value.value());
    }

    ticket_t getTicket()
    {
        return impl::get_ticket(m_value);
    }
    bool setConditional(T* value, const ticket_t& condition)
    {
        return impl::set_conditional(&m_value, reinterpret_cast< value_t >(value), condition);
    }
};

template < typename T >
class istack
{
public:
    struct node
    {
        node* next;
        node() : next(0)
        {
        }
        node(const node& /*other*/) : next(0)
        {
        }
    };

private:
    itaggedptr< node > m_head;

public:
    istack();
    ~istack();

    void push(T* t);
    void pushList(T* h, T* t);
    T*   pop();
};

template < typename T >
istack< T >::istack() : m_head(0)
{
}

template < typename T >
istack< T >::~istack()
{
}

template < typename T >
void istack< T >::push(T* t)
{
    typename itaggedptr< node >::ticket_t ticket;
    do
    {
        ticket  = m_head.getTicket();
        t->next = static_cast< T* >((node*)m_head);
    } while(!m_head.setConditional(t, ticket));
}

template < typename T >
void istack< T >::pushList(T* h, T* t)
{
    typename itaggedptr< node >::ticket_t ticket;
    do
    {
        ticket  = m_head.getTicket();
        t->next = static_cast< T* >((node*)m_head);
    } while(!m_head.setConditional(h, ticket));
}

template < typename T >
T* istack< T >::pop()
{
    typename itaggedptr< node >::ticket_t ticket;
    T*                                    result;
    do
    {
        ticket = m_head.getTicket();
        result = static_cast< T* >((node*)m_head);
    } while(result && !m_head.setConditional(const_cast< node* >(result->next), ticket));
    return result;
}

}  // namespace minitl

/**************************************************************************************************/
#endif
