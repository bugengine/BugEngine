/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_KERNEL_GCC_ARM64_INTERLOCKED_INL_
#define BE_KERNEL_GCC_ARM64_INTERLOCKED_INL_
/**************************************************************************************************/
#include <bugengine/kernel/stdafx.h>

/*
 * most implementations here from Hans Boehm's atomic ops
 * http://www.hpl.hp.com/research/linux/atomic_ops/
 */
#if defined(__thumb__) && !defined(__thumb2__)
/* Thumb One mode does not have ARM "mcr", "swp" and some load/store  */
/* instructions, so we temporarily switch to ARM mode and go back     */
/* afterwards (clobbering "r3" register).                             */
#    define AO_THUMB_GO_ARM                                                                        \
        "       adr     r3, 4f\n"                                                                  \
        "       bx      r3\n"                                                                      \
        "      .align\n"                                                                           \
        "      .arm\n"                                                                             \
        "4:\n"
#    define AO_THUMB_RESTORE_MODE                                                                  \
        "       adr     r3, 5f + 1\n"                                                              \
        "       bx      r3\n"                                                                      \
        "       .thumb\n"                                                                          \
        "5:\n"
#    define AO_THUMB_SWITCH_CLOBBERS "x3",
#else
#    define AO_THUMB_GO_ARM          /* empty */
#    define AO_THUMB_RESTORE_MODE    /* empty */
#    define AO_THUMB_SWITCH_CLOBBERS /* empty */
#endif                               /* !__thumb__ */

namespace _Kernel {

template < unsigned size >
struct InterlockedType;

template <>
struct InterlockedType< 4 >
{
    typedef int           value_t;
    static inline value_t fetch_and_add(value_t* p, value_t incr)
    {
        value_t old = 0;
        value_t temp, flag;
        __asm__ __volatile__(AO_THUMB_GO_ARM "       dmb sy\n"
                                             "1:     ldaxr   %w0, %5\n"
                                             "       add     %w2, %w0, %w4\n"
                                             "       stxr    %w1, %w2, %5\n"
                                             "       cmp             %w1, #0\n"
                                             "       b.ne            1b\n"
                                             "       dmb st\n" AO_THUMB_RESTORE_MODE
                             : "=&r"(old), "=&r"(flag), "=&r"(temp), "+m"(*p)
                             : "r"(incr), "Q"(*p)
                             : AO_THUMB_SWITCH_CLOBBERS "cc");
        return old;
    }
    static inline value_t fetch_and_sub(value_t* p, value_t incr)
    {
        return fetch_and_add(p, -incr);
    }
    static inline value_t fetch_and_set(value_t* p, value_t v)
    {
        value_t prev, flag;
        __asm__ __volatile__(AO_THUMB_GO_ARM "       dmb sy\n"
                                             "1:     ldaxr   %w0, %3\n"
                                             "       stxr    %w1, %w4, %3\n"
                                             "       cmp             %w1, #0\n"
                                             "       b.ne            1b\n"
                                             "       dmb st\n" AO_THUMB_RESTORE_MODE
                             : "=&r"(prev), "=&r"(flag), "+m"(*p)
                             : "Q"(*p), "r"(v)
                             : AO_THUMB_SWITCH_CLOBBERS "cc");
        return prev;
    }
    static inline value_t set_conditional(value_t* p, value_t v, value_t condition)
    {
        value_t result, old;
        __asm__ __volatile__(AO_THUMB_GO_ARM
                             "       dmb sy\n"
                             "1:     ldaxr           %w1, %3\n"      /* get original */
                             "       cmp             %w1, %w4\n"     /* see if match */
                             "       bne             2f\n"           /* if not, end */
                             "       stxr            %w0, %w5, %3\n" /* store new one if matched */
                             "       cmp             %w0, #1\n"
                             "       beq             1b\n" /* if update failed, repeat */
                             "2:     dmb st\n" AO_THUMB_RESTORE_MODE
                             : "=&r"(result), "=&r"(old), "+m"(*p)
                             : "Q"(*p), "r"(condition), "r"(v)
                             : AO_THUMB_SWITCH_CLOBBERS "cc");

        return old;
    }
    static inline value_t set_and_fetch(value_t* p, value_t v)
    {
        fetch_and_set(p, v);
        return v;
    }

    struct tagged_t
    {
        typedef void* value_t;
        typedef void* tag_t;

        __attribute__((aligned(4))) value_t m_value;

        tagged_t(value_t value = 0) : m_value(value)
        {
        }
        tagged_t(const tagged_t& other) : m_value(other.m_value)
        {
        }
        tagged_t& operator=(const tagged_t& other)
        {
            m_value = other.m_value;
            return *this;
        }
        inline value_t value()
        {
            return m_value;
        }
        inline bool operator==(tagged_t& other)
        {
            return m_value == other.m_value;
        }
    };
    static inline tagged_t::tag_t get_ticket(const tagged_t& p)
    {
        tagged_t::value_t result;
        __asm__ __volatile__(AO_THUMB_GO_ARM "       dmb sy\n"
                                             "       ldaxr   %w0, %1\n" AO_THUMB_RESTORE_MODE
                             : "=r"(result)
                             : "Q"(p.m_value)
                             : AO_THUMB_SWITCH_CLOBBERS "cc");
        return result;
    }
    static inline bool set_conditional(tagged_t* p, tagged_t::value_t v,
                                       const tagged_t::tag_t& /*condition*/)
    {
        long result;

        __asm__ __volatile__(AO_THUMB_GO_ARM "       stxr %w0, %w2, %3\n"
                                             "       dmb st\n" AO_THUMB_RESTORE_MODE
                             : "=&r"(result), "+m"(*p)
                             : "r"(v), "Q"(*p)
                             : AO_THUMB_SWITCH_CLOBBERS "cc");

        return !(result);
    }
};

template <>
struct InterlockedType< 8 >
{
    typedef long long     value_t;
    static inline value_t fetch_and_add(value_t* p, value_t incr)
    {
        value_t old = 0;
        value_t temp, flag;
        __asm__ __volatile__(AO_THUMB_GO_ARM "       dmb sy\n"
                                             "1:     ldaxr   %x0, %5\n"
                                             "       add     %x2, %x0, %x4\n"
                                             "       stlxr    %w1, %x2, %5\n"
                                             "       cmp             %w1, #0\n"
                                             "       b.ne            1b\n"
                                             "       dmb st\n" AO_THUMB_RESTORE_MODE
                             : "=&r"(old), "=&r"(flag), "=&r"(temp), "+m"(*p)
                             : "r"(incr), "Q"(*p)
                             : AO_THUMB_SWITCH_CLOBBERS "cc");
        return old;
    }
    static inline value_t fetch_and_sub(value_t* p, value_t incr)
    {
        return fetch_and_add(p, -incr);
    }
    static inline value_t fetch_and_set(value_t* p, value_t v)
    {
        value_t prev, flag;
        __asm__ __volatile__(AO_THUMB_GO_ARM "       dmb sy\n"
                                             "1:     ldaxr    %x0, %3\n"
                                             "       stlxr    %w1, %x4, %3\n"
                                             "       cmp             %w1, #0\n"
                                             "       b.ne             1b\n"
                                             "       dmb st\n" AO_THUMB_RESTORE_MODE
                             : "=&r"(prev), "=&r"(flag), "+m"(*p)
                             : "Q"(*p), "r"(v)
                             : AO_THUMB_SWITCH_CLOBBERS "cc");
        return prev;
    }
    static inline value_t set_conditional(value_t* p, value_t v, value_t condition)
    {
        value_t result, old;
        __asm__ __volatile__(AO_THUMB_GO_ARM
                             "       dmb sy\n"
                             "1:     ldaxr    %x1, %3\n"  /* get original */
                             "       cmp      %x1, %x4\n" /* see if match */
                             "       b.ne     2f\n"
                             "       stlxr    %w0, %x5, %3\n" /* store new one if matched */
                             "       cbnz     %w0, 1b\n"      /* if update failed, repeat */
                             "2:     dmb st\n" AO_THUMB_RESTORE_MODE
                             : "=&r"(result), "=&r"(old), "+m"(*p)
                             : "Q"(*p), "r"(condition), "r"(v)
                             : AO_THUMB_SWITCH_CLOBBERS "cc");

        return old;
    }
    static inline value_t set_and_fetch(value_t* p, value_t v)
    {
        fetch_and_set(p, v);
        return v;
    }

    struct tagged_t
    {
        typedef void* value_t;
        typedef void* tag_t;

        __attribute__((aligned(8))) value_t m_value;

        tagged_t(value_t value = 0) : m_value(value)
        {
        }
        tagged_t(const tagged_t& other) : m_value(other.m_value)
        {
        }
        tagged_t& operator=(const tagged_t& other)
        {
            m_value = other.m_value;
            return *this;
        }
        inline value_t value()
        {
            return m_value;
        }
        inline bool operator==(tagged_t& other)
        {
            return m_value == other.m_value;
        }
    };
    static inline tagged_t::tag_t get_ticket(const tagged_t& p)
    {
        tagged_t::value_t result;
        __asm__ __volatile__(AO_THUMB_GO_ARM "       dmb sy\n"
                                             "       ldaxr   %x0, %1\n" AO_THUMB_RESTORE_MODE
                             : "=r"(result)
                             : "Q"(p.m_value)
                             : AO_THUMB_SWITCH_CLOBBERS "cc");
        return result;
    }
    static inline bool set_conditional(tagged_t* p, tagged_t::value_t v,
                                       const tagged_t::tag_t& /*condition*/)
    {
        long result;

        __asm__ __volatile__(AO_THUMB_GO_ARM "       stlxr  %x0, %x2, %3\n"
                                             "       dmb st\n" AO_THUMB_RESTORE_MODE
                             : "=&r"(result), "+m"(*p)
                             : "r"(v), "Q"(*p)
                             : AO_THUMB_SWITCH_CLOBBERS "cc");

        return !(result);
    }
};

template <>
struct InterlockedType< 1 > : public InterlockedType< 4 >
{
};

template <>
struct InterlockedType< 2 > : public InterlockedType< 4 >
{
};

}  // namespace _Kernel

/**************************************************************************************************/
#endif
