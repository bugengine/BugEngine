/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_INTERLOCKED_GCC_PPC64_INTERLOCKED_INL_
#define BE_MINITL_INTERLOCKED_GCC_PPC64_INTERLOCKED_INL_
/*****************************************************************************/

namespace minitl { namespace interlocked_impl
{

template< unsigned size >
struct InterlockedType;
	
template<>
struct InterlockedType<8>
{
    typedef long value_t;
    static inline value_t fetch_and_add(volatile value_t *p, value_t incr)
    {
        value_t old = 0;
        value_t temp;
        __asm__ __volatile__ (" lwsync\n"
                              "retry%=:\n"
                              " ldarx  %0, 0, %3\n"
                              " add    %1, %2, %0\n"
                              " stdcx. %1, 0, %3\n"
                              " bne- retry%=\n"
                              " isync\n"
                              : "=&r" (old), "=&r"(temp)
                              : "r" (incr), "r" (p)
                              : "memory", "cc");
        return old;
    }
    static inline value_t fetch_and_sub(volatile value_t *p, value_t incr)
    {
        return fetch_and_add(p, -incr);
    }
    static inline value_t fetch_and_set(volatile value_t *p, value_t v)
    {
        value_t prev;
        __asm__ __volatile__ (" lwsync\n"
                              "retry%=:\n"
                              " ldarx  %0, 0, %2\n"
                              " stdcx. %1, 0, %2\n"
                              " bne- retry%=\n"
                              " isync\n"
                              : "=&r" (prev)
                              : "r" (v), "r" (p)
                              : "memory", "cc");
        return prev;
    }
    static inline value_t set_conditional(volatile value_t *p, value_t v, value_t condition)
    {
        value_t result;
        __asm__ __volatile__ (" lwsync\n"
                              "retry3%=:\n"
                              " ldarx  %0, 0, %3\n"
                              " cmpw   %2, %0\n"
                              " bne-  exit%=\n"
                              " stdcx. %1, 0, %3\n"
                              " bne- retry%=\n"
                              " isync\n"
                              "exit%=:\n"
                              : "=&r" (result)
                              : "r" (v), "r"(condition), "r" (p)
                              : "memory", "cc");
        return result;
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
        __asm__ __volatile__ ("  lwsync\n"
                              "  ldarx %0, 0, %1\n"
                              : "=r"(result)
                              : "r"(&p.m_value)
                              : "cc");
        return result;
    }
    static inline bool set_conditional(volatile tagged_t *p, value_t v, tagged_t::tag_t& condition)
    {
        bool result;
        __asm__ __volatile__("  li %0,0\n"
                             "  stdcx. %1, 0, %2\n"
                             "  bne- end%=\n"
                             "  li %0,1\n"
                             "end%=:\n"
                             "  isync\n"
                             :  "=&r"(result)
                             :  "r"(v), "r"(p)
                             :  "memory", "cc");
        return result;
    }
};
	
template<>
struct InterlockedType<4>
{
    typedef int value_t;
    static inline value_t fetch_and_add(volatile value_t *p, value_t incr)
    {
        value_t old = 0;
        value_t temp;
        __asm__ __volatile__ (" lwsync\n"
                              "retry%=:\n"
                              " lwarx  %0, 0, %3\n"
                              " add    %1, %2, %0\n"
                              " stwcx. %1, 0, %3\n"
                              " bne- retry%=\n"
                              " isync\n"
                      : "=&r" (old), "=&r"(temp)
                      : "r" (incr), "r" (p)
                      : "memory", "cc");
        return old;
    }
    static inline value_t fetch_and_sub(volatile value_t *p, value_t incr)
    {
        return fetch_and_add(p, -incr);
    }
    static inline value_t fetch_and_set(volatile value_t *p, value_t v)
    {
        value_t prev;
        __asm__ __volatile__ (" lwsync\n"
                              "retry%=:\n"
                              " lwarx  %0, 0, %2\n"
                              " stwcx. %1, 0, %2\n"
                              " bne- retry%=\n"
                              " isync\n"
                      : "=&r" (prev)
                      : "r" (v), "r" (p)
                      : "memory", "cc");
        return prev;
    }
    static inline value_t set_conditional(volatile value_t *p, value_t v, value_t condition)
    {
        value_t result;
        __asm__ __volatile__ (" lwsync\n"
                              "retry3%=:\n"
                              " lwarx  %0, 0, %3\n"
                              " cmpw   %2, %0\n"
                              " bne-  exit%=\n"
                              " stwcx. %1, 0, %3\n"
                              " bne- retry%=\n"
                              " isync\n"
                              "exit%=:\n"
                      : "=&r" (result)
                      : "r" (v), "r"(condition), "r" (p)
                      : "memory", "cc");
        return result;
    }
    static inline value_t set_and_fetch(volatile value_t *p, value_t v)
    {
        fetch_and_set(p, v);
        return v;
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
