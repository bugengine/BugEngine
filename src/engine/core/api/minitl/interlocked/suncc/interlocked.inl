/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_INTERLOCKED_SUNCC_X86_INTERLOCKED_INL_
#define BE_MINITL_INTERLOCKED_SUNCC_X86_INTERLOCKED_INL_
/*****************************************************************************/

namespace minitl { namespace interlocked_impl
{

template< unsigned size >
struct InterlockedType;


template<>
struct InterlockedType<4>
{
    typedef i32 value_t;
    static inline value_t fetch_and_add(volatile value_t *p, value_t incr)
    {
        value_t old=0;
        __asm__ __volatile__ ("lock; xadd %0,%1"
                      : "=a" (old), "=m" (*p)
                      : "a" (incr), "m" (*p)
                      : "memory", "cc");
        return old;
    }
    static inline value_t fetch_and_sub(volatile value_t *p, value_t incr)
    {
        return fetch_and_add(p, -incr);
    }
    static inline value_t fetch_and_set(volatile value_t *p, value_t v)
    {
        __asm__ __volatile__ ("lock; xchg %2, %1"
                      : "=r" (v), "+m" (*p)
                      : "r" (v));
        return v;
    }
    static inline value_t set_conditional(volatile value_t *p, value_t v, value_t condition)
    {
        __asm__ __volatile__ ("lock; cmpxchg %1, %2"
                      : "=r" (v)
                      : "r" (v), "m" (*(p)), "0"(condition)
                      : "memory", "cc");
        return v;
    }
    static inline value_t set_and_fetch(volatile value_t *p, value_t v)
    {
        fetch_and_set(p, v);
        return v;
    }

    struct tagged_t
    {
        typedef i32         value_t;
        typedef i32         counter_t;
        typedef tagged_t    tag_t;
        struct
        {
            volatile counter_t   tag;
            volatile value_t     value;
        } taggedvalue;

        tagged_t(value_t value = 0)
        {
            taggedvalue.tag = 0;
            taggedvalue.value = value;
        }
        tagged_t(counter_t tag, value_t value)
        {
            taggedvalue.tag = tag;
            taggedvalue.value = value;
        }
        tagged_t(const tagged_t& other)
        {
            taggedvalue.tag = other.taggedvalue.tag;
            taggedvalue.value = other.taggedvalue.value;
        }
        tagged_t& operator=(const tagged_t& other)
        {
            taggedvalue.tag = other.taggedvalue.tag;
            taggedvalue.value = other.taggedvalue.value;
            return *this;
        }
        inline value_t value() { return taggedvalue.value; }
        inline bool operator==(tagged_t& other) { return (taggedvalue.tag == other.taggedvalue.tag) && (taggedvalue.value == other.taggedvalue.value); }
    };
    static inline tagged_t::tag_t get_ticket(const tagged_t &p)
    {
        return p;
    }
    static inline bool set_conditional(volatile tagged_t *p, tagged_t::value_t v, tagged_t::tag_t& condition)
    {
        tagged_t result;
        tagged_t dst(condition.taggedvalue.tag+1, v);
        __asm__ __volatile__ (
                "lock;  cmpxchg8b %2\n\t"
                 : "=a"(result.taggedvalue.tag), "=d"(result.taggedvalue.value), "=m"(*p)
                 : "a"(condition.taggedvalue.tag), "d"(condition.taggedvalue.value), "b"(dst.taggedvalue.tag), "c"(v)
                 : "memory", "cc");
        return result.taggedvalue.tag == condition.taggedvalue.tag;
    }
};

template<>
struct InterlockedType<8>
{
    typedef i64 value_t;
    static inline value_t fetch_and_add(volatile value_t *p, value_t incr)
    {
        value_t old = 0;
        __asm__ __volatile__ ("lock; xaddq %0,%1"
                      : "=a" (old), "=m" (*p)
                      : "a" (incr), "m" (*p)
                      : "memory", "cc");
        return old;
    }
    static inline value_t fetch_and_sub(volatile value_t *p, value_t incr)
    {
        return fetch_and_add(p, -incr);
    }
    static inline value_t fetch_and_set(volatile value_t *p, value_t v)
    {
        __asm__ __volatile__ ("lock; xchgq %2, %1"
                      : "=r" (v), "+m" (*p)
                      : "r" (v));
        return v;
    }
    static inline value_t set_conditional(volatile value_t *p, value_t v, value_t condition)
    {
    #ifdef __PIC__
        __asm__ __volatile__("push %%rbx\n\t" : : : "esp");
    #endif
        value_t result = 0;
        union split {
            i64 asI64;
            i32 asI32[2];
        };
        split dst;
        dst.asI64 = v;
        split src;
        src.asI64 = condition;

        __asm__ __volatile__ (
                "lock;  cmpxchg8b %1\n\t"
                 : "=A"(result), "=m"(*p)
                 : "m"(*p), "a"(src.asI32[0]), "d"(src.asI32[1]), "b"(dst.asI32[0]), "c"(dst.asI32[1])
                 : "memory"

        );
    #ifdef __PIC__
        __asm__ __volatile__("pop %%rbx\n\t" : : : "esp");
    #endif
        return result;
    }
    static inline value_t set_and_fetch(volatile value_t *p, value_t v)
    {
        fetch_and_set(p, v);
        return v;
    }


    struct tagged_t
    {
        typedef i64         value_t;
        typedef i64         counter_t;
        typedef tagged_t    tag_t;
        struct
        {
            volatile counter_t   tag;
            volatile value_t     value;
        } taggedvalue;

        tagged_t(value_t value = 0)
        {
            taggedvalue.tag = 0;
            taggedvalue.value = value;
        }
        tagged_t(counter_t tag, value_t value)
        {
            taggedvalue.tag = tag;
            taggedvalue.value = value;
        }
        tagged_t(const tagged_t& other)
        {
            taggedvalue.tag = other.taggedvalue.tag;
            taggedvalue.value = other.taggedvalue.value;
        }
        tagged_t& operator=(const tagged_t& other)
        {
            taggedvalue.tag = other.taggedvalue.tag;
            taggedvalue.value = other.taggedvalue.value;
            return *this;
        }
        inline value_t value() { return taggedvalue.value; }
        inline bool operator==(tagged_t& other) { return (taggedvalue.tag == other.taggedvalue.tag) && (taggedvalue.value == other.taggedvalue.value); }
    };
    static inline tagged_t::tag_t get_ticket(const tagged_t &p)
    {
        return p;
    }
    static inline bool set_conditional(volatile tagged_t *p, tagged_t::value_t v, tagged_t::tag_t& condition)
    {
        unsigned char result = 0;
        __asm__ __volatile__ (
                "\tmov %2,%%r8\n"
                "\t.byte 0xF0,0x49,0x0F,0xC7,0x08\n"
                "\tsetz %0\n"
                 : "=a"(result), "=m"(*p)
                 : "r"(p), "d"(condition.taggedvalue.value), "a"(condition.taggedvalue.tag), "c"(v), "b"(condition.taggedvalue.tag+1)
                 : "memory"
        );
        return result;
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
