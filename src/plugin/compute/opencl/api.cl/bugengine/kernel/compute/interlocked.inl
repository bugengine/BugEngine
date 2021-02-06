/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_KERNEL_CL_COMPUTE_INTERLOCKED_INL_
#define BE_KERNEL_CL_COMPUTE_INTERLOCKED_INL_
/**************************************************************************************************/
#include <bugengine/kernel/stdafx.h>

namespace _Kernel {

template < unsigned size >
struct InterlockedType;

template <>
struct InterlockedType< 4 >
{
    typedef i32 value_t;

    static inline __host __device value_t fetch_and_add(kernel_global value_t* p, value_t incr);
    static inline __host __device value_t fetch_and_sub(kernel_global value_t* p, value_t incr);

    static inline __host __device value_t set_conditional(kernel_global value_t* p, value_t v,
                                                          value_t condition);
    static inline __host __device value_t set_and_fetch(kernel_global value_t* p, value_t v);
    static inline __host __device value_t fetch_and_set(kernel_global value_t* p, value_t v);

    static inline __host __device value_t fetch_and_add(kernel_local value_t* p, value_t incr);
    static inline __host __device value_t fetch_and_sub(kernel_local value_t* p, value_t incr);

    static inline __host __device value_t set_conditional(kernel_local value_t* p, value_t v,
                                                          value_t condition);
    static inline __host __device value_t set_and_fetch(kernel_local value_t* p, value_t v);
    static inline __host __device value_t fetch_and_set(kernel_local value_t* p, value_t v);

    /* not defined, host only */
    struct tagged_t
    {
        typedef void* value_t;
        typedef void* tag_t;
    };
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

template <>
struct InterlockedType< 8 >
{
    typedef u64 value_t;

    static inline __host __device value_t fetch_and_add(kernel_global value_t* p, value_t incr);
    static inline __host __device value_t fetch_and_sub(kernel_global value_t* p, value_t incr);

    static inline __host __device value_t fetch_and_set(kernel_global value_t* p, value_t v);
    static inline __host __device value_t set_conditional(kernel_global value_t* p, value_t v,
                                                          value_t condition);
    static inline __host __device value_t set_and_fetch(kernel_global value_t* p, value_t v);

    static inline __host __device value_t fetch_and_add(kernel_local value_t* p, value_t incr);
    static inline __host __device value_t fetch_and_sub(kernel_local value_t* p, value_t incr);

    static inline __host __device value_t fetch_and_set(kernel_local value_t* p, value_t v);
    static inline __host __device value_t set_conditional(kernel_local value_t* p, value_t v,
                                                          value_t condition);
    static inline __host __device value_t set_and_fetch(kernel_local value_t* p, value_t v);

    /* not defined, host only */
    struct tagged_t
    {
        typedef void* value_t;
        typedef void* tag_t;
    };
    static inline tagged_t::tag_t get_ticket(const tagged_t& p);
    static inline bool            set_conditional(tagged_t* p, tagged_t::value_t v,
                                                  const tagged_t::tag_t& /*condition*/);
};

}  // namespace _Kernel

namespace _Kernel {

__device InterlockedType< 4 >::value_t InterlockedType< 4 >::fetch_and_add(kernel_global value_t* p,
                                                                           value_t incr)
{
    return atomic_add(p, incr);
}

__device InterlockedType< 4 >::value_t InterlockedType< 4 >::fetch_and_sub(kernel_global value_t* p,
                                                                           value_t incr)
{
    return atomic_sub(p, incr);
}

__device InterlockedType< 4 >::value_t InterlockedType< 4 >::fetch_and_set(kernel_global value_t* p,
                                                                           value_t                v)
{
    return atomic_xchg(p, v);
}

__device InterlockedType< 4 >::value_t
InterlockedType< 4 >::set_conditional(kernel_global value_t* p, value_t v, value_t condition)
{
    return atomic_cmpxchg(p, condition, v);
}

__device InterlockedType< 4 >::value_t InterlockedType< 4 >::set_and_fetch(kernel_global value_t* p,
                                                                           value_t                v)
{
    fetch_and_set(p, v);
    return v;
}

__device InterlockedType< 4 >::value_t InterlockedType< 4 >::fetch_and_add(kernel_local value_t* p,
                                                                           value_t incr)
{
    return atomic_add(p, incr);
}

__device InterlockedType< 4 >::value_t InterlockedType< 4 >::fetch_and_sub(kernel_local value_t* p,
                                                                           value_t incr)
{
    return atomic_sub(p, incr);
}

__device InterlockedType< 4 >::value_t InterlockedType< 4 >::fetch_and_set(kernel_local value_t* p,
                                                                           value_t               v)
{
    return atomic_xchg(p, v);
}

__device InterlockedType< 4 >::value_t
InterlockedType< 4 >::set_conditional(kernel_local value_t* p, value_t v, value_t condition)
{
    return atomic_cmpxchg(p, condition, v);
}

__device InterlockedType< 4 >::value_t InterlockedType< 4 >::set_and_fetch(kernel_local value_t* p,
                                                                           value_t               v)
{
    fetch_and_set(p, v);
    return v;
}

__device InterlockedType< 8 >::value_t InterlockedType< 8 >::fetch_and_add(kernel_global value_t* p,
                                                                           value_t incr)
{
    return atom_add(p, incr);
}

__device InterlockedType< 8 >::value_t InterlockedType< 8 >::fetch_and_sub(kernel_global value_t* p,
                                                                           value_t incr)
{
    return atom_sub(p, incr);
}

__device InterlockedType< 8 >::value_t InterlockedType< 8 >::fetch_and_set(kernel_global value_t* p,
                                                                           value_t                v)
{
    return atom_xchg(p, v);
}

__device InterlockedType< 8 >::value_t
InterlockedType< 8 >::set_conditional(kernel_global value_t* p, value_t v, value_t condition)
{
    return atom_cmpxchg(p, condition, v);
}

__device InterlockedType< 8 >::value_t InterlockedType< 8 >::set_and_fetch(kernel_global value_t* p,
                                                                           value_t                v)
{
    fetch_and_set(p, v);
    return v;
}

__device InterlockedType< 8 >::value_t InterlockedType< 8 >::fetch_and_add(kernel_local value_t* p,
                                                                           value_t incr)
{
    return atom_add(p, incr);
}

__device InterlockedType< 8 >::value_t InterlockedType< 8 >::fetch_and_sub(kernel_local value_t* p,
                                                                           value_t incr)
{
    return atom_sub(p, incr);
}

__device InterlockedType< 8 >::value_t InterlockedType< 8 >::fetch_and_set(kernel_local value_t* p,
                                                                           value_t               v)
{
    return atom_xchg(p, v);
}

__device InterlockedType< 8 >::value_t
InterlockedType< 8 >::set_conditional(kernel_local value_t* p, value_t v, value_t condition)
{
    return atom_cmpxchg(p, condition, v);
}

__device InterlockedType< 8 >::value_t InterlockedType< 8 >::set_and_fetch(kernel_local value_t* p,
                                                                           value_t               v)
{
    fetch_and_set(p, v);
    return v;
}

}  // namespace _Kernel

/**************************************************************************************************/
#endif
