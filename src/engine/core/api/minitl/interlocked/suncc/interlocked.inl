/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_INTERLOCKED_SUNCC_INTERLOCKED_INL_
#define BE_MINITL_INTERLOCKED_SUNCC_INTERLOCKED_INL_
/*****************************************************************************/
#include    <core/debug/assert.hh>
#include    <atomic.h>

namespace minitl { namespace interlocked_
{

template< unsigned size >
struct InterlockedType;

template<>
struct InterlockedType<1>
{
    typedef long type;
};
template<>
struct InterlockedType<2>
{
    typedef long type;
};
template<>
struct InterlockedType<4>
{
    typedef long type;
};
template<>
struct InterlockedType<8>
{
    typedef long long type;
};
struct BE_SET_ALIGNMENT(4) TaggedValue
{
    typedef long        ValueType;
    typedef TaggedValue TagType;
    union
    {
        struct
        {
            long    tag;
            long    value;
        };
        long long asLongLong;
    };
    TaggedValue(long value = 0)
        :   tag(0)
        ,   value(value)
    {
    }
    TaggedValue(const volatile TaggedValue& other)
        :   tag(other.tag)
        ,   value(other.value)
    {
    }
    TaggedValue(const TaggedValue& other)
        :   tag(other.tag)
        ,   value(other.value)
    {
    }
    TaggedValue& operator=(const TaggedValue& other)
    {
        tag = other.tag;
        value = other.value;
        return *this;
    }
    inline bool operator==(TaggedValue& other) { return tag == other.tag && value == other.value; }
};

inline long set_conditional(volatile long* dst, long compare, long value)
{
}

inline long long set_conditional(volatile long long* dst, long long compare, long long value)
{
    long long result;
    return result;
}

inline void* set_conditional(void* volatile* dst, void* compare, void* value)
{
    void* prev;
    __asm__ volatile ("lock; cmpxchgl %1, %2"
                  : "=a" (prev)
                  : "r" (value), "m" (*(dst)), "0"(compare)
                  : "memory", "cc");
    return prev;
}

inline TaggedValue get_ticket(volatile TaggedValue* t)
{
    be_assert((((size_t)t) & 7) == 0, "wrong alignment : type );
    return *t;
}

inline bool set_conditional(volatile TaggedValue* t, long value, TaggedValue& ticket)
{
    be_assert((((size_t)t) & 7) == 0);
    long long m = (((long long)value) << 32) + ticket.tag+1;
    return set_conditional((volatile long long*)t, ticket.asLongLong, m) == ticket.asLongLong;
}

inline long set_and_fetch(volatile long* dst, long value)
{
    *dst = value;
    return value;
}

inline long long set_and_fetch(volatile long long* dst, long long value)
{
    *dst = value;
    return value;
}

inline void* set_and_fetch(void * volatile * dst, void* value)
{
    *dst = value;
    return value;
}


inline long fetch_and_set(volatile long* dst, long value)
{
    long prev;
    __asm__ __volatile__ ("xchgl %0, %1"
                  : "=a" (prev), "+m" (*dst)
                  : "r" (value));
    return prev;
}

inline long long fetch_and_set(volatile long long* dst, long long value)
{
    be_unimplemented();
    return 0;
}

inline void* fetch_and_set(void * volatile * dst, void* value)
{
    void* prev;
    __asm__ __volatile__ ("xchgl %0, %1"
                  : "=a" (prev), "+m" (*dst)
                  : "r" (value));
    return prev;
}


inline long fetch_and_add(volatile long* dst, long value)
{
    long old;
    __asm__ __volatile__ ("lock; xaddl %0,%1"
                  : "=a" (old), "=m" (*dst)
                  : "a" (value), "m" (*dst)
                  : "memory", "cc");
    return old;
}

inline long long fetch_and_add(volatile long long* dst, long long value)
{
    be_unimplemented(false);
    return 0;
}


inline long add_and_fetch(volatile long* dst, long value)
{
    long old;
    __asm__ __volatile__ ("lock; xaddl %0,%1"
                  : "=a" (old), "=m" (*dst)
                  : "a" (value), "m" (*dst)
                  : "memory", "cc");
    return old+value;
}

inline long long add_and_fetch(volatile long long* dst, long long value)
{
    be_unimplemented();
    return 0;
}


inline long fetch_and_sub(volatile long* dst, long value)
{
    return fetch_and_add(dst, -value);
}

inline long long fetch_and_sub(volatile long long* dst, long long value)
{
    return fetch_and_add(dst, -value);
}


inline long sub_and_fetch(volatile long* dst, long value)
{
    return add_and_fetch(dst, -value);
}

inline long long sub_and_fetch(volatile long long* dst, long long value)
{
    return add_and_fetch(dst, -value);
}


inline long or_and_fetch(volatile long* dst, long value)
{
    be_unimplemented();
    return 0;
}

inline long long or_and_fetch(volatile long long* dst, long long value)
{
    be_unimplemented();
    return 0;
}


inline long xor_and_fetch(volatile long* dst, long value)
{
    be_unimplemented();
    return 0;
}

inline long long xor_and_fetch(volatile long long* dst, long long value)
{
    be_unimplemented();
    return 0;
}


inline long and_and_fetch(volatile long* dst, long value)
{
    be_unimplemented();
    return 0;
}

inline long long and_and_fetch(volatile long long* dst, long long value)
{
    be_unimplemented();
    return 0;
}

}}

/*****************************************************************************/
#endif

