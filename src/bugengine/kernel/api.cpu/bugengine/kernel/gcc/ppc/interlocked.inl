/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_KERNEL_GCC_PPC_INTERLOCKED_INL_
#define BE_KERNEL_GCC_PPC_INTERLOCKED_INL_
/**************************************************************************************************/
#include <bugengine/kernel/stdafx.h>

namespace _Kernel {

template < unsigned size >
struct InterlockedType;

template <>
struct InterlockedType< 4 >
{
    typedef int value_t;

    struct tagged_t
    {
        typedef void* value_t;
        typedef void* tag_t;

        __attribute__((aligned(4))) value_t m_value;

        inline tagged_t(value_t value = 0);
        inline tagged_t(const tagged_t& other);
        inline tagged_t& operator=(const tagged_t& other);
        inline value_t   value();
        inline bool      operator==(tagged_t& other);
    };

    static inline value_t         fetch_and_add(value_t* p, value_t incr);
    static inline value_t         fetch_and_sub(value_t* p, value_t incr);
    static inline value_t         fetch_and_set(value_t* p, value_t v);
    static inline value_t         set_conditional(value_t* p, value_t v, value_t condition);
    static inline value_t         set_and_fetch(value_t* p, value_t v);
    static inline tagged_t::tag_t get_ticket(const tagged_t& p);
    static inline bool            set_conditional(tagged_t* p, tagged_t::value_t v,
                                                  const tagged_t::tag_t& /*condition*/);
};

template <>
struct InterlockedType< 1 > : public InterlockedType< 4 >
{
};

template <>
struct InterlockedType< 2 > : public InterlockedType< 4 >
{
};

#ifdef _PPC64
template <>
struct InterlockedType< 8 >
{
    typedef long long int value_t;

    struct tagged_t
    {
        typedef void*   value_t;
        typedef value_t tag_t;

        __attribute__((aligned(8))) value_t m_value;

        inline tagged_t(value_t value = 0);
        inline tagged_t(const tagged_t& other);
        inline tagged_t& operator=(const tagged_t& other);
        inline value_t   value();
        inline bool      operator==(tagged_t& other);
    };

    static inline value_t fetch_and_add(value_t* p, value_t incr);
    static inline value_t fetch_and_sub(value_t* p, value_t incr);
    static inline value_t fetch_and_set(value_t* p, value_t v);
    static inline value_t set_conditional(value_t* p, value_t v, value_t condition);
    static inline value_t set_and_fetch(value_t* p, value_t v);

    static inline tagged_t::tag_t get_ticket(const tagged_t& p);
    static inline bool            set_conditional(tagged_t* p, tagged_t::value_t v,
                                                  const tagged_t::tag_t& /*condition*/);
};
#endif

}  // namespace _Kernel

namespace _Kernel {

InterlockedType< 4 >::tagged_t::tagged_t(value_t value) : m_value(value)
{
}

InterlockedType< 4 >::tagged_t::tagged_t(const tagged_t& other) : m_value(other.m_value)
{
}

InterlockedType< 4 >::tagged_t& InterlockedType< 4 >::tagged_t::operator=(const tagged_t& other)
{
    m_value = other.m_value;
    return *this;
}

InterlockedType< 4 >::tagged_t::value_t InterlockedType< 4 >::tagged_t::value()
{
    return m_value;
}

bool InterlockedType< 4 >::tagged_t::operator==(tagged_t& other)
{
    return m_value == other.m_value;
}

InterlockedType< 4 >::value_t InterlockedType< 4 >::fetch_and_add(value_t* p, value_t incr)
{
    value_t old = 0;
    value_t temp;
    __asm__ __volatile__(" lwsync\n"
                         "1:\n"
                         " lwarx  %0, 0, %3\n"
                         " add    %1, %2, %0\n"
                         " stwcx. %1, 0, %3\n"
                         " bne 1b\n"
                         " isync\n"
                         : "=&r"(old), "=&r"(temp)
                         : "r"(incr), "r"(p)
                         : "memory", "cc");
    return old;
}

InterlockedType< 4 >::value_t InterlockedType< 4 >::fetch_and_sub(value_t* p, value_t incr)
{
    return fetch_and_add(p, -incr);
}

InterlockedType< 4 >::value_t InterlockedType< 4 >::fetch_and_set(value_t* p, value_t v)
{
    value_t prev;
    __asm__ __volatile__(" lwsync\n"
                         "1:\n"
                         " lwarx  %0, 0, %2\n"
                         " stwcx. %1, 0, %2\n"
                         " bne 1b\n"
                         " isync\n"
                         : "=&r"(prev)
                         : "r"(v), "r"(p)
                         : "memory", "cc");
    return prev;
}

InterlockedType< 4 >::value_t InterlockedType< 4 >::set_conditional(value_t* p, value_t v,
                                                                    value_t condition)
{
    value_t result;
    __asm__ __volatile__(" lwsync\n"
                         "1:\n"
                         " lwarx  %0, 0, %3\n"
                         " cmpw   %2, %0\n"
                         " bne  2f\n"
                         " stwcx. %1, 0, %3\n"
                         " bne 1b\n"
                         " isync\n"
                         "2:\n"
                         : "=&r"(result)
                         : "r"(v), "r"(condition), "r"(p)
                         : "memory", "cc");
    return result;
}

InterlockedType< 4 >::value_t InterlockedType< 4 >::set_and_fetch(value_t* p, value_t v)
{
    fetch_and_set(p, v);
    return v;
}

InterlockedType< 4 >::tagged_t::tag_t InterlockedType< 4 >::get_ticket(const tagged_t& p)
{
    tagged_t::value_t result;
    __asm__ __volatile__("  lwsync\n"
                         "  lwarx %0, 0, %1\n"
                         : "=r"(result)
                         : "r"(&p.m_value)
                         : "cc");
    return result;
}

bool InterlockedType< 4 >::set_conditional(tagged_t* p, tagged_t::value_t v,
                                           const tagged_t::tag_t& /*condition*/)
{
    bool result;
    __asm__ __volatile__("  li %0,0\n"
                         "  stwcx. %1, 0, %2\n"
                         "  bne  1f\n"
                         "  li %0,1\n"
                         "1:\n"
                         "  isync\n"
                         : "=&r"(result)
                         : "r"(v), "r"(p)
                         : "memory", "cc");
    return result;
}

#ifdef _PPC64

InterlockedType< 8 >::tagged_t::tagged_t(value_t value) : m_value(value)
{
}

InterlockedType< 8 >::tagged_t::tagged_t(const tagged_t& other) : m_value(other.m_value)
{
}

InterlockedType< 8 >::tagged_t& InterlockedType< 8 >::tagged_t::operator=(const tagged_t& other)
{
    m_value = other.m_value;
    return *this;
}

InterlockedType< 8 >::tagged_t::value_t InterlockedType< 8 >::tagged_t::value()
{
    return m_value;
}

bool InterlockedType< 8 >::tagged_t::operator==(tagged_t& other)
{
    return m_value == other.m_value;
}

InterlockedType< 8 >::value_t InterlockedType< 8 >::fetch_and_add(value_t* p, value_t incr)
{
    value_t old = 0;
    value_t temp;
    __asm__ __volatile__(" lwsync\n"
                         "1:\n"
                         " ldarx  %0, 0, %3\n"
                         " add    %1, %2, %0\n"
                         " stdcx. %1, 0, %3\n"
                         " bne 1b\n"
                         " isync\n"
                         : "=&r"(old), "=&r"(temp)
                         : "r"(incr), "r"(p)
                         : "memory", "cc");
    return old;
}

InterlockedType< 8 >::value_t InterlockedType< 8 >::fetch_and_sub(value_t* p, value_t incr)
{
    return fetch_and_add(p, -incr);
}

InterlockedType< 8 >::value_t InterlockedType< 8 >::fetch_and_set(value_t* p, value_t v)
{
    value_t prev;
    __asm__ __volatile__(" lwsync\n"
                         "1:\n"
                         " ldarx  %0, 0, %2\n"
                         " stdcx. %1, 0, %2\n"
                         " bne 1b\n"
                         " isync\n"
                         : "=&r"(prev)
                         : "r"(v), "r"(p)
                         : "memory", "cc");
    return prev;
}

InterlockedType< 8 >::value_t InterlockedType< 8 >::set_conditional(value_t* p, value_t v,
                                                                    value_t condition)
{
    value_t result;
    __asm__ __volatile__(" lwsync\n"
                         "1:\n"
                         " ldarx  %0, 0, %3\n"
                         " cmpw   %2, %0\n"
                         " bne 2f\n"
                         " stdcx. %1, 0, %3\n"
                         " bne 1b\n"
                         " isync\n"
                         "2:\n"
                         : "=&r"(result)
                         : "r"(v), "r"(condition), "r"(p)
                         : "memory", "cc");
    return result;
}

InterlockedType< 8 >::value_t InterlockedType< 8 >::set_and_fetch(value_t* p, value_t v)
{
    fetch_and_set(p, v);
    return v;
}

InterlockedType< 8 >::tagged_t::tag_t InterlockedType< 8 >::get_ticket(const tagged_t& p)
{
    tagged_t::value_t result;
    __asm__ __volatile__("  lwsync\n"
                         "  ldarx %0, 0, %1\n"
                         : "=r"(result)
                         : "r"(&p.m_value)
                         : "cc");
    return result;
}

bool InterlockedType< 8 >::set_conditional(tagged_t* p, tagged_t::value_t v,
                                           const tagged_t::tag_t& /*condition*/)
{
    bool result;
    __asm__ __volatile__("  li %0,0\n"
                         "  stdcx. %1, 0, %2\n"
                         "  bne 1f\n"
                         "  li %0,1\n"
                         "1:\n"
                         "  isync\n"
                         : "=&r"(result)
                         : "r"(v), "r"(p)
                         : "memory", "cc");
    return result;
}

#endif

}  // namespace _Kernel

/**************************************************************************************************/
#endif
