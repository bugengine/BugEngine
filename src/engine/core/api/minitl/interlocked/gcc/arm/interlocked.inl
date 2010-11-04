/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_INTERLOCKED_GCC_PPC_INTERLOCKED_INL_
#define BE_MINITL_INTERLOCKED_GCC_PPC_INTERLOCKED_INL_
/*****************************************************************************/

/*
 * most implementations here from Hans Boehm's atomic ops
 * http://www.hpl.hp.com/research/linux/atomic_ops/
 */

namespace minitl { namespace interlocked_impl
{

template< unsigned size >
struct InterlockedType;

template<>
struct InterlockedType<4>
{
    typedef long value_t;
    static inline value_t fetch_and_add(volatile value_t *p, value_t incr)
    {
        value_t old = 0;
        value_t temp, flag;
        __asm__ __volatile__(
                "1:     ldrex   %0, [%5]\n"
                "       add     %2, %0, %4\n"
                "       strex   %1, %2, [%5]\n"
                "       teq             %1, #0\n"
                "       bne             1b\n"
            : "=&r"(old),"=&r"(flag),"=&r"(temp),"+m"(*p)
            : "r"(incr), "r"(p)
            : "cc");
        return old;
    }
    static inline value_t fetch_and_sub(volatile value_t *p, value_t incr)
    {
        return fetch_and_add(p, -incr);
    }
    static inline value_t fetch_and_set(volatile value_t *p, value_t v)
    {
        value_t prev, flag;
        __asm__ __volatile__(
                "1:     ldrex   %0, [%3]\n"
                "       strex   %1, %4, [%3]\n"
                "       teq             %1, #0\n"
                "       bne             1b\n"
            : "=&r"(prev),"=&r"(flag), "+m"(*p)
            : "r"(p), "r"(v)
            : "cc");
        return prev;
    }
    static inline value_t set_conditional(volatile value_t *p, value_t v, value_t condition)
    {
        value_t result, old;
        __asm__ __volatile__ (
                "1:     mov             %0, #2\n"                       /* store a flag */
                "       ldrex   %1, [%3]\n"                     /* get original */
                "       teq             %1, %4\n"                       /* see if match */
                "       strexeq %0, %5, [%3]\n"         /* store new one if matched */
                "       teq             %0, #1\n"
                "       beq             1b\n"                           /* if update failed, repeat */
            : "=&r"(result), "=&r"(old), "+m"(*p)
            : "r"(p), "r"(condition), "r"(v)
            : "cc");

        return old;
    }
    static inline value_t set_and_fetch(volatile value_t *p, value_t v)
    {
        fetch_and_set(p, v);
        return v;
    }

    struct tagged_t
    {
        typedef long        value_t;
        typedef value_t     tag_t;
        
        BE_SET_ALIGNMENT(4) volatile value_t     m_value;

        tagged_t(long value = 0)
            :   m_value(value)
        {
        }
        tagged_t(const tagged_t& other)
            :   m_value(other.m_value)
        {
        }
        tagged_t& operator=(const tagged_t& other)
        {
            m_value = other.m_value;
            return *this;
        }
        inline value_t value() { return m_value; }
        inline bool operator==(tagged_t& other) { return m_value == other.m_value; }
    };
    static inline tagged_t::tag_t get_ticket(const tagged_t &p)
    {
        value_t result;
        __asm__ __volatile__ (
                "       ldrex   %0, [%1]\n"
            : "=r"(result)
            : "r"(&p.m_value)
            : "cc");
        return result;
    }
    static inline bool set_conditional(volatile tagged_t *p, value_t v, tagged_t::tag_t& condition)
    {
         long result;

        __asm__ __volatile__(
                "       strex %0, %2, [%3]\n"
            : "=&r"(result), "+m"(*p)
            : "r"(v), "r"(p)
            : "cc");

        return !(result&2);  
    }
};

template<>
struct InterlockedType<1> : public InterlockedType<4>
{
};

template<>
struct InterlockedType<2> : public InterlockedType<4>
{
};


}}

/*****************************************************************************/
#endif
