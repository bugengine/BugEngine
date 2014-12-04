/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_KERNEL_SUNCC_INTERLOCKED_INL_
#define BE_KERNEL_SUNCC_INTERLOCKED_INL_
/**************************************************************************************************/
#include    <kernel/stdafx.h>

extern "C" i32 fetch_and_add_32(i32* p, i32 add);
extern "C" i32 fetch_and_set_32(i32* p, i32 v);
extern "C" i32 set_conditional_32(i32* p, i32 v, i32 condition);
extern "C" i64 set_conditional_64(i64* p, i64 v, i64 condition);
#ifdef _AMD64
extern "C" i64 fetch_and_add_64(i64* p, i64 add);
extern "C" i64 fetch_and_set_64(i64* p, i64 v);
extern "C" char set_conditional_128(i64* p, i64 nvalue, i64 oldvalue, i64 tag);
#endif

namespace _Kernel
{

template< unsigned size >
struct InterlockedType;


template<>
struct InterlockedType<4>
{
    typedef i32 value_t;
    static inline value_t fetch_and_add(value_t *p, value_t incr)
    {
        return fetch_and_add_32(p, incr);
    }
    static inline value_t fetch_and_sub(value_t *p, value_t incr)
    {
        return fetch_and_add_32(p, -incr);
    }
    static inline value_t fetch_and_set(value_t *p, value_t v)
    {
        return fetch_and_set_32(p, v);
    }

    static inline value_t set_conditional(value_t *p, value_t v, value_t condition)
    {
        return set_conditional_32(p, v, condition);
    }
    static inline value_t set_and_fetch(value_t *p, value_t v)
    {
        fetch_and_set_32(p, v);
        return v;
    }

    struct tagged_t
    {
        typedef void*       value_t;
        typedef i32         counter_t;
        typedef tagged_t    tag_t;
        struct
        {
            counter_t   tag;
            value_t     value;
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
    #pragma struct_align 8 (tagged_t)
    static inline tagged_t::tag_t get_ticket(const tagged_t &p)
    {
        return p;
    }
    static inline bool set_conditional(tagged_t *p, tagged_t::value_t v,
                                       const tagged_t::tag_t& condition)
    {
        i64 old = *(i64*)&condition;
        i64 value = *(i64*)&v;
        i64* dest = (i64*)p;
        return set_conditional_64(dest, value, old) == old;
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


#ifdef _AMD64
template<>
struct InterlockedType<8>
{
    typedef i64 value_t;
    static inline value_t fetch_and_add(value_t *p, value_t incr)
    {
        return fetch_and_add_64(p, incr);
    }
    static inline value_t fetch_and_sub(value_t *p, value_t incr)
    {
        return fetch_and_add_64(p, -incr);
    }
    static inline value_t fetch_and_set(value_t *p, value_t v)
    {
        return fetch_and_set_64(p, v);
    }
    static inline value_t set_conditional(value_t *p, value_t v, value_t condition)
    {
        return set_conditional_64(p, v, condition);
    }
    static inline value_t set_and_fetch(value_t *p, value_t v)
    {
        fetch_and_set_64(p, v);
        return v;
    }


    struct tagged_t
    {
        typedef void*       value_t;
        typedef i64         counter_t;
        typedef tagged_t    tag_t;
        struct
        {
            counter_t   tag;
            value_t     value;
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
    #pragma struct_align 16 (tagged_t)
    static inline tagged_t::tag_t get_ticket(const tagged_t &p)
    {
        return p;
    }
    static inline bool set_conditional(tagged_t *p, tagged_t::value_t v, tagged_t::tag_t& condition)
    {
        return set_conditional_128((i64*)p, (i64)v, condition.taggedvalue.tag,
                                   (i64)condition.taggedvalue.value);
    }
};
#endif


}

/**************************************************************************************************/
#endif
