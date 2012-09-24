/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_THREADS_QUEUE_HH_
#define BE_CORE_THREADS_QUEUE_HH_
/*****************************************************************************/
#include    <core/threads/semaphore.hh>
#include    <kernel/interlocked.hh>

namespace BugEngine
{

template< typename T >
class Queue
{
public:
    struct Node
    {
        friend class Queue<T>;
    private:
        itaggedptr<Node>    next;
        Node*               previous;
    protected:
        Node()
            :   next(0)
            ,   previous(0)
        {
        }
    };
private:
    Semaphore           m_counter;
    Node                m_head;
    itaggedptr<Node>    m_tail;
    i_u8                m_exit;
public:
    Queue();
    ~Queue();

    void push(T* t);
    T* pop();
};

}

#include    <core/threads/queue.inl>

/*****************************************************************************/
#endif
