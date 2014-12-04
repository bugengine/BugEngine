/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_KERNEL_GCC_MIPS_INTERLOCKED_INL_
#define BE_KERNEL_GCC_MIPS_INTERLOCKED_INL_
/**************************************************************************************************/
#include    <kernel/stdafx.h>

namespace _Kernel
{

template< unsigned size >
struct InterlockedType;


template<>
struct InterlockedType<4>
{
    typedef __attribute__ ((aligned(4))) i32 value_t;
    static inline value_t fetch_and_add(volatile value_t *p, value_t incr)
    {
        value_t old = *p;
        *p += incr;
        return old;
    }
    static inline value_t fetch_and_sub(volatile value_t *p, value_t incr)
    {
        return fetch_and_add(p, -incr);
    }
    static inline value_t fetch_and_set(volatile value_t *p, value_t v)
    {
        value_t old = *p;
        *p = v;
        return old;
    }
    static inline value_t set_conditional(volatile value_t *p, value_t v, value_t condition)
    {
        value_t old = *p;
        if (old == condition)
        {
            *p = v;
        }
        return old;
    }
    static inline value_t set_and_fetch(volatile value_t *p, value_t v)
    {
        fetch_and_set(p, v);
        return v;
    }

    struct tagged_t
    {
        typedef __attribute__ ((aligned(4))) void*  value_t;
        typedef __attribute__ ((aligned(4))) i32    counter_t;
        typedef counter_t    tag_t;
        __attribute__ ((aligned(8))) struct
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
        inline bool operator==(tagged_t& other)
        {
            return (taggedvalue.tag == other.taggedvalue.tag)
                && (taggedvalue.value == other.taggedvalue.value);
        }
    };
    static inline tagged_t::tag_t get_ticket(const tagged_t &p)
    {
        return p.taggedvalue.tag;
    }
    static inline bool set_conditional(volatile tagged_t *p, tagged_t::value_t v,
                                       const tagged_t::tag_t& condition)
    {
        if (p->taggedvalue.tag == condition)
        {
            p->taggedvalue.tag ++;
            p->taggedvalue.value = v;
            return true;
        }
        return false;
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


#ifdef _MIPS64
template<>
struct InterlockedType<8>
{
    typedef __attribute__ ((aligned(8))) i64 value_t;
    static inline value_t fetch_and_add(volatile value_t *p, value_t incr)
    {
        value_t old = *p;
        *p += incr;
        return old;
    }
    static inline value_t fetch_and_sub(volatile value_t *p, value_t incr)
    {
        return fetch_and_add(p, -incr);
    }
    static inline value_t fetch_and_set(volatile value_t *p, value_t v)
    {
        value_t old = *p;
        *p = v;
        return old;
    }
    static inline value_t set_conditional(volatile value_t *p, value_t v, value_t condition)
    {
        value_t old = *p;
        if (old == condition)
        {
            *p = v;
        }
        return old;
    }
    static inline value_t set_and_fetch(volatile value_t *p, value_t v)
    {
        fetch_and_set(p, v);
        return v;
    }


    struct tagged_t
    {
        typedef __attribute__ ((aligned(8))) void*  value_t;
        typedef __attribute__ ((aligned(8))) i64    counter_t;
        typedef counter_t    tag_t;
        __attribute__ ((aligned(16))) struct
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
        inline bool operator==(tagged_t& other)
        {
            return (taggedvalue.tag == other.taggedvalue.tag)
                && (taggedvalue.value == other.taggedvalue.value);
        }
    };
    static inline tagged_t::tag_t get_ticket(const tagged_t &p)
    {
        return p.taggedvalue.tag;
    }
    static inline bool set_conditional(volatile tagged_t *p, tagged_t::value_t v,
                                       const tagged_t::tag_t& condition)
    {
        if (p->taggedvalue.tag == condition)
        {
            p->taggedvalue.tag ++;
            p->taggedvalue.value = v;
            return true;
        }
        return false;
    }
};
#endif


}

/**************************************************************************************************/
#endif
