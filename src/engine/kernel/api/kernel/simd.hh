/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_KERNEL_SIMD_HH_
#define BE_KERNEL_SIMD_HH_
/*****************************************************************************/

namespace _Kernel
{
template< typename T, unsigned ROW, unsigned COLUMN >
struct array;

template< typename T, unsigned ROW >
struct array<T, ROW, 1>
{
    T   t[ROW];
    T& operator[](u32 index)
    {
        return t[index];
    }
    T operator[](u32 index) const
    {
        return t[index];
    }
};

template< typename T, unsigned COLUMN >
struct array<T, 1, COLUMN>
{
    T   t[COLUMN];
    T& operator[](u32 index)
    {
        return t[index];
    }
    T operator[](u32 index) const
    {
        return t[index];
    }
};

template< typename T, unsigned ROW, unsigned COLUMN >
struct array
{
	array<T, 1, COLUMN> rows[ROW];
    array<T, 1, COLUMN>& operator[](u32 index)
    {
        return rows[index];
    }
    const array<T, 1, COLUMN>& operator[](u32 index) const
    {
        return rows[index];
    }
};

}

typedef _Kernel::array<u8, 1, 2>          byte1x2;
static inline byte1x2 make_byte1x2(u8 x1, u8 x2)
{
    byte1x2 result = {{x1, x2}};
    return result;
}
typedef _Kernel::array<u8, 1, 3>          byte1x3;
static inline byte1x3 make_byte1x3(u8 x1, u8 x2, u8 x3)
{
    byte1x3 result = {{x1, x2, x3}};
    return result;
}
typedef _Kernel::array<u8, 1, 4>          byte1x4;
static inline byte1x4 make_byte1x4(u8 x1, u8 x2, u8 x3, u8 x4)
{
    byte1x4 result = {{x1, x2, x3, x4}};
    return result;
}

typedef _Kernel::array<u8, 2, 1>          byte2;
static inline byte2 make_byte2(u8 x1, u8 x2)
{
    byte2 result = {{x1, x2}};
    return result;
}
typedef _Kernel::array<u8, 2, 2>          byte2x2;
static inline byte2x2 make_byte2x2(u8 x1, u8 x2, u8 x3, u8 x4)
{
    byte2x2 result = {{{{x1, x2}}, {{x3, x4}}}};
    return result;
}
typedef _Kernel::array<u8, 2, 3>          byte2x3;
static inline byte2x3 make_byte2x3(u8 x1, u8 x2, u8 x3, u8 x4, u8 x5, u8 x6)
{
    byte2x3 result = {{{{x1, x2, x3}}, {{x4, x5, x6}}}};
    return result;
}
typedef _Kernel::array<u8, 2, 4>          byte2x4;
static inline byte2x4 make_byte2x4(u8 x1, u8 x2, u8 x3, u8 x4, u8 x5, u8 x6, u8 x7, u8 x8)
{
    byte2x4 result = {{{{x1, x2, x3, x4}}, {{x5, x6, x7, x8}}}};
    return result;
}

typedef _Kernel::array<u8, 3, 1>          byte3;
static inline byte3 make_byte3(u8 x1, u8 x2, u8 x3)
{
    byte3 result = {{x1, x2, x3}};
    return result;
}
typedef _Kernel::array<u8, 3, 2>          byte3x2;
static inline byte3x2 make_byte3x2(u8 x1, u8 x2, u8 x3, u8 x4, u8 x5, u8 x6)
{
    byte3x2 result = {{{{x1, x2}}, {{x3, x4}}, {{x5, x6}}}};
    return result;
}
typedef _Kernel::array<u8, 3, 3>          byte3x3;
static inline byte3x3 make_byte3x3(u8 x1, u8 x2, u8 x3, u8 x4, u8 x5, u8 x6, u8 x7, u8 x8, u8 x9)
{
    byte3x3 result = {{{{x1, x2, x3}}, {{x4, x5, x6}}, {{x7, x8, x9}}}};
    return result;
}
typedef _Kernel::array<u8, 3, 4>          byte3x4;
static inline byte3x4 make_byte3x4(u8 x1, u8 x2, u8 x3, u8 x4, u8 x5, u8 x6, u8 x7, u8 x8, u8 x9, u8 x10, u8 x11, u8 x12)
{
    byte3x4 result = {{{{x1, x2, x3, x4}}, {{x5, x6, x7, x8}}, {{x9, x10, x11, x12}}}};
    return result;
}

typedef _Kernel::array<u8, 4, 1>          byte4;
static inline byte4 make_byte4(u8 x1, u8 x2, u8 x3, u8 x4)
{
    byte4 result = {{x1, x2, x3, x4}};
    return result;
}
typedef _Kernel::array<u8, 4, 2>          byte4x2;
static inline byte4x2 make_byte4x2(u8 x1, u8 x2, u8 x3, u8 x4, u8 x5, u8 x6, u8 x7, u8 x8)
{
    byte4x2 result = {{{{x1, x2}}, {{x3, x4}}, {{x5, x6}}, {{x7, x8}}}};
    return result;
}
typedef _Kernel::array<u8, 4, 3>          byte4x3;
static inline byte4x3 make_byte4x3(u8 x1, u8 x2, u8 x3, u8 x4, u8 x5, u8 x6, u8 x7, u8 x8, u8 x9, u8 x10, u8 x11, u8 x12)
{
    byte4x3 result = {{{{x1, x2, x3}}, {{x4, x5, x6}}, {{x7, x8, x9}}, {{x10, x11, x12}}}};
    return result;
}
typedef _Kernel::array<u8, 4, 4>          byte4x4;
static inline byte4x4 make_byte4x4(u8 x1, u8 x2, u8 x3, u8 x4, u8 x5, u8 x6, u8 x7, u8 x8, u8 x9, u8 x10, u8 x11, u8 x12, u8 x13, u8 x14, u8 x15, u8 x16)
{
    byte4x4 result = {{{{x1, x2, x3, x4}}, {{x5, x6, x7, x8}}, {{x9, x10, x11, x12}}, {{x13, x14, x15, x16}}}};
    return result;
}


typedef _Kernel::array<i16, 1, 2>         short1x2;
static inline short1x2 make_short1x2(i16 x1, i16 x2)
{
    short1x2 result = {{x1, x2}};
    return result;
}
typedef _Kernel::array<i16, 1, 3>         short1x3;
static inline short1x3 make_short1x3(i16 x1, i16 x2, i16 x3)
{
    short1x3 result = {{x1, x2, x3}};
    return result;
}
typedef _Kernel::array<i16, 1, 4>         short1x4;
static inline short1x4 make_short1x4(i16 x1, i16 x2, i16 x3, i16 x4)
{
    short1x4 result = {{x1, x2, x3, x4}};
    return result;
}

typedef _Kernel::array<i16, 2, 1>         short2;
static inline short2 make_short2(i16 x1, i16 x2)
{
    short2 result = {{x1, x2}};
    return result;
}
typedef _Kernel::array<i16, 2, 2>         short2x2;
static inline short2x2 make_short2x2(i16 x1, i16 x2, i16 x3, i16 x4)
{
    short2x2 result = {{{{x1, x2}}, {{x3, x4}}}};
    return result;
}
typedef _Kernel::array<i16, 2, 3>         short2x3;
static inline short2x3 make_short2x3(i16 x1, i16 x2, i16 x3, i16 x4, i16 x5, i16 x6)
{
    short2x3 result = {{{{x1, x2, x3}}, {{x4, x5, x6}}}};
    return result;
}
typedef _Kernel::array<i16, 2, 4>         short2x4;
static inline short2x4 make_short2x4(i16 x1, i16 x2, i16 x3, i16 x4, i16 x5, i16 x6, i16 x7, i16 x8)
{
    short2x4 result = {{{{x1, x2, x3, x4}}, {{x5, x6, x7, x8}}}};
    return result;
}

typedef _Kernel::array<i16, 3, 1>         short3;
static inline short3 make_short3(i16 x1, i16 x2, i16 x3)
{
    short3 result = {{x1, x2, x3}};
    return result;
}
typedef _Kernel::array<i16, 3, 2>         short3x2;
static inline short3x2 make_short3x2(i16 x1, i16 x2, i16 x3, i16 x4, i16 x5, i16 x6)
{
    short3x2 result = {{{{x1, x2}}, {{x3, x4}}, {{x5, x6}}}};
    return result;
}
typedef _Kernel::array<i16, 3, 3>         short3x3;
static inline short3x3 make_short3x3(i16 x1, i16 x2, i16 x3, i16 x4, i16 x5, i16 x6, i16 x7, i16 x8, i16 x9)
{
    short3x3 result = {{{{x1, x2, x3}}, {{x4, x5, x6}}, {{x7, x8, x9}}}};
    return result;
}
typedef _Kernel::array<i16, 3, 4>         short3x4;
static inline short3x4 make_short3x4(i16 x1, i16 x2, i16 x3, i16 x4, i16 x5, i16 x6, i16 x7, i16 x8, i16 x9, i16 x10, i16 x11, i16 x12)
{
    short3x4 result = {{{{x1, x2, x3, x4}}, {{x5, x6, x7, x8}}, {{x9, x10, x11, x12}}}};
    return result;
}

typedef _Kernel::array<i16, 4, 1>         short4;
static inline short4 make_short4(i16 x1, i16 x2, i16 x3, i16 x4)
{
    short4 result = {{x1, x2, x3, x4}};
    return result;
}
typedef _Kernel::array<i16, 4, 2>         short4x2;
static inline short4x2 make_short4x2(i16 x1, i16 x2, i16 x3, i16 x4, i16 x5, i16 x6, i16 x7, i16 x8)
{
    short4x2 result = {{{{x1, x2}}, {{x3, x4}}, {{x5, x6}}, {{x7, x8}}}};
    return result;
}
typedef _Kernel::array<i16, 4, 3>         short4x3;
static inline short4x3 make_short4x3(i16 x1, i16 x2, i16 x3, i16 x4, i16 x5, i16 x6, i16 x7, i16 x8, i16 x9, i16 x10, i16 x11, i16 x12)
{
    short4x3 result = {{{{x1, x2, x3}}, {{x4, x5, x6}}, {{x7, x8, x9}}, {{x10, x11, x12}}}};
    return result;
}
typedef _Kernel::array<i16, 4, 4>         short4x4;
static inline short4x4 make_short4x4(i16 x1, i16 x2, i16 x3, i16 x4, i16 x5, i16 x6, i16 x7, i16 x8, i16 x9, i16 x10, i16 x11, i16 x12, i16 x13, i16 x14, i16 x15, i16 x16)
{
    short4x4 result = {{{{x1, x2, x3, x4}}, {{x5, x6, x7, x8}}, {{x9, x10, x11, x12}}, {{x13, x14, x15, x16}}}};
    return result;
}


typedef _Kernel::array<u16, 1, 2>         ushort1x2;
static inline ushort1x2 make_ushort1x2(u16 x1, u16 x2)
{
    ushort1x2 result = {{x1, x2}};
    return result;
}
typedef _Kernel::array<u16, 1, 3>         ushort1x3;
static inline ushort1x3 make_ushort1x3(u16 x1, u16 x2, u16 x3)
{
    ushort1x3 result = {{x1, x2, x3}};
    return result;
}
typedef _Kernel::array<u16, 1, 4>         ushort1x4;
static inline ushort1x4 make_ushort1x4(u16 x1, u16 x2, u16 x3, u16 x4)
{
    ushort1x4 result = {{x1, x2, x3, x4}};
    return result;
}

typedef _Kernel::array<u16, 2, 1>         ushort2;
static inline ushort2 make_ushort2(u16 x1, u16 x2)
{
    ushort2 result = {{x1, x2}};
    return result;
}
typedef _Kernel::array<u16, 2, 2>         ushort2x2;
static inline ushort2x2 make_ushort2x2(u16 x1, u16 x2, u16 x3, u16 x4)
{
    ushort2x2 result = {{{{x1, x2}}, {{x3, x4}}}};
    return result;
}
typedef _Kernel::array<u16, 2, 3>         ushort2x3;
static inline ushort2x3 make_ushort2x3(u16 x1, u16 x2, u16 x3, u16 x4, u16 x5, u16 x6)
{
    ushort2x3 result = {{{{x1, x2, x3}}, {{x4, x5, x6}}}};
    return result;
}
typedef _Kernel::array<u16, 2, 4>         ushort2x4;
static inline ushort2x4 make_ushort2x4(u16 x1, u16 x2, u16 x3, u16 x4, u16 x5, u16 x6, u16 x7, u16 x8)
{
    ushort2x4 result = {{{{x1, x2, x3, x4}}, {{x5, x6, x7, x8}}}};
    return result;
}

typedef _Kernel::array<u16, 3, 1>         ushort3;
static inline ushort3 make_ushort3(u16 x1, u16 x2, u16 x3)
{
    ushort3 result = {{x1, x2, x3}};
    return result;
}
typedef _Kernel::array<u16, 3, 2>         ushort3x2;
static inline ushort3x2 make_ushort3x2(u16 x1, u16 x2, u16 x3, u16 x4, u16 x5, u16 x6)
{
    ushort3x2 result = {{{{x1, x2}}, {{x3, x4}}, {{x5, x6}}}};
    return result;
}
typedef _Kernel::array<u16, 3, 3>         ushort3x3;
static inline ushort3x3 make_ushort3x3(u16 x1, u16 x2, u16 x3, u16 x4, u16 x5, u16 x6, u16 x7, u16 x8, u16 x9)
{
    ushort3x3 result = {{{{x1, x2, x3}}, {{x4, x5, x6}}, {{x7, x8, x9}}}};
    return result;
}
typedef _Kernel::array<u16, 3, 4>         ushort3x4;
static inline ushort3x4 make_ushort3x4(u16 x1, u16 x2, u16 x3, u16 x4, u16 x5, u16 x6, u16 x7, u16 x8, u16 x9, u16 x10, u16 x11, u16 x12)
{
    ushort3x4 result = {{{{x1, x2, x3, x4}}, {{x5, x6, x7, x8}}, {{x9, x10, x11, x12}}}};
    return result;
}

typedef _Kernel::array<u16, 4, 1>         ushort4;
static inline ushort4 make_ushort4(u16 x1, u16 x2, u16 x3, u16 x4)
{
    ushort4 result = {{x1, x2, x3, x4}};
    return result;
}
typedef _Kernel::array<u16, 4, 2>         ushort4x2;
static inline ushort4x2 make_ushort4x2(u16 x1, u16 x2, u16 x3, u16 x4, u16 x5, u16 x6, u16 x7, u16 x8)
{
    ushort4x2 result = {{{{x1, x2}}, {{x3, x4}}, {{x5, x6}}, {{x7, x8}}}};
    return result;
}
typedef _Kernel::array<u16, 4, 3>         ushort4x3;
static inline ushort4x3 make_ushort4x3(u16 x1, u16 x2, u16 x3, u16 x4, u16 x5, u16 x6, u16 x7, u16 x8, u16 x9, u16 x10, u16 x11, u16 x12)
{
    ushort4x3 result = {{{{x1, x2, x3}}, {{x4, x5, x6}}, {{x7, x8, x9}}, {{x10, x11, x12}}}};
    return result;
}
typedef _Kernel::array<u16, 4, 4>         ushort4x4;
static inline ushort4x4 make_ushort4x4(u16 x1, u16 x2, u16 x3, u16 x4, u16 x5, u16 x6, u16 x7, u16 x8, u16 x9, u16 x10, u16 x11, u16 x12, u16 x13, u16 x14, u16 x15, u16 x16)
{
    ushort4x4 result = {{{{x1, x2, x3, x4}}, {{x5, x6, x7, x8}}, {{x9, x10, x11, x12}}, {{x13, x14, x15, x16}}}};
    return result;
}


typedef _Kernel::array<i32, 1, 2>         int1x2;
static inline int1x2 make_int1x2(i32 x1, i32 x2)
{
    int1x2 result = {{x1, x2}};
    return result;
}
typedef _Kernel::array<i32, 1, 3>         int1x3;
static inline int1x3 make_int1x3(i32 x1, i32 x2, i32 x3)
{
    int1x3 result = {{x1, x2, x3}};
    return result;
}
typedef _Kernel::array<i32, 1, 4>         int1x4;
static inline int1x4 make_int1x4(i32 x1, i32 x2, i32 x3, i32 x4)
{
    int1x4 result = {{x1, x2, x3, x4}};
    return result;
}

typedef _Kernel::array<i32, 2, 1>         int2;
static inline int2 make_int2(i32 x1, i32 x2)
{
    int2 result = {{x1, x2}};
    return result;
}
typedef _Kernel::array<i32, 2, 2>         int2x2;
static inline int2x2 make_int2x2(i32 x1, i32 x2, i32 x3, i32 x4)
{
    int2x2 result = {{{{x1, x2}}, {{x3, x4}}}};
    return result;
}
typedef _Kernel::array<i32, 2, 3>         int2x3;
static inline int2x3 make_int2x3(i32 x1, i32 x2, i32 x3, i32 x4, i32 x5, i32 x6)
{
    int2x3 result = {{{{x1, x2, x3}}, {{x4, x5, x6}}}};
    return result;
}
typedef _Kernel::array<i32, 2, 4>         int2x4;
static inline int2x4 make_int2x4(i32 x1, i32 x2, i32 x3, i32 x4, i32 x5, i32 x6, i32 x7, i32 x8)
{
    int2x4 result = {{{{x1, x2, x3, x4}}, {{x5, x6, x7, x8}}}};
    return result;
}

typedef _Kernel::array<i32, 3, 1>         int3;
static inline int3 make_int3(i32 x1, i32 x2, i32 x3)
{
    int3 result = {{x1, x2, x3}};
    return result;
}
typedef _Kernel::array<i32, 3, 2>         int3x2;
static inline int3x2 make_int3x2(i32 x1, i32 x2, i32 x3, i32 x4, i32 x5, i32 x6)
{
    int3x2 result = {{{{x1, x2}}, {{x3, x4}}, {{x5, x6}}}};
    return result;
}
typedef _Kernel::array<i32, 3, 3>         int3x3;
static inline int3x3 make_int3x3(i32 x1, i32 x2, i32 x3, i32 x4, i32 x5, i32 x6, i32 x7, i32 x8, i32 x9)
{
    int3x3 result = {{{{x1, x2, x3}}, {{x4, x5, x6}}, {{x7, x8, x9}}}};
    return result;
}
typedef _Kernel::array<i32, 3, 4>         int3x4;
static inline int3x4 make_int3x4(i32 x1, i32 x2, i32 x3, i32 x4, i32 x5, i32 x6, i32 x7, i32 x8, i32 x9, i32 x10, i32 x11, i32 x12)
{
    int3x4 result = {{{{x1, x2, x3, x4}}, {{x5, x6, x7, x8}}, {{x9, x10, x11, x12}}}};
    return result;
}

typedef _Kernel::array<i32, 4, 1>         int4;
static inline int4 make_int4(i32 x1, i32 x2, i32 x3, i32 x4)
{
    int4 result = {{x1, x2, x3, x4}};
    return result;
}
typedef _Kernel::array<i32, 4, 2>         int4x2;
static inline int4x2 make_int4x2(i32 x1, i32 x2, i32 x3, i32 x4, i32 x5, i32 x6, i32 x7, i32 x8)
{
    int4x2 result = {{{{x1, x2}}, {{x3, x4}}, {{x5, x6}}, {{x7, x8}}}};
    return result;
}
typedef _Kernel::array<i32, 4, 3>         int4x3;
static inline int4x3 make_int4x3(i32 x1, i32 x2, i32 x3, i32 x4, i32 x5, i32 x6, i32 x7, i32 x8, i32 x9, i32 x10, i32 x11, i32 x12)
{
    int4x3 result = {{{{x1, x2, x3}}, {{x4, x5, x6}}, {{x7, x8, x9}}, {{x10, x11, x12}}}};
    return result;
}
typedef _Kernel::array<i32, 4, 4>         int4x4;
static inline int4x4 make_int4x4(i32 x1, i32 x2, i32 x3, i32 x4, i32 x5, i32 x6, i32 x7, i32 x8, i32 x9, i32 x10, i32 x11, i32 x12, i32 x13, i32 x14, i32 x15, i32 x16)
{
    int4x4 result = {{{{x1, x2, x3, x4}}, {{x5, x6, x7, x8}}, {{x9, x10, x11, x12}}, {{x13, x14, x15, x16}}}};
    return result;
}


typedef _Kernel::array<u32, 1, 2>         uint1x2;
static inline uint1x2 make_uint1x2(u32 x1, u32 x2)
{
    uint1x2 result = {{x1, x2}};
    return result;
}
typedef _Kernel::array<u32, 1, 3>         uint1x3;
static inline uint1x3 make_uint1x3(u32 x1, u32 x2, u32 x3)
{
    uint1x3 result = {{x1, x2, x3}};
    return result;
}
typedef _Kernel::array<u32, 1, 4>         uint1x4;
static inline uint1x4 make_uint1x4(u32 x1, u32 x2, u32 x3, u32 x4)
{
    uint1x4 result = {{x1, x2, x3, x4}};
    return result;
}

typedef _Kernel::array<u32, 2, 1>         uint2;
static inline uint2 make_uint2(u32 x1, u32 x2)
{
    uint2 result = {{x1, x2}};
    return result;
}
typedef _Kernel::array<u32, 2, 2>         uint2x2;
static inline uint2x2 make_uint2x2(u32 x1, u32 x2, u32 x3, u32 x4)
{
    uint2x2 result = {{{{x1, x2}}, {{x3, x4}}}};
    return result;
}
typedef _Kernel::array<u32, 2, 3>         uint2x3;
static inline uint2x3 make_uint2x3(u32 x1, u32 x2, u32 x3, u32 x4, u32 x5, u32 x6)
{
    uint2x3 result = {{{{x1, x2, x3}}, {{x4, x5, x6}}}};
    return result;
}
typedef _Kernel::array<u32, 2, 4>         uint2x4;
static inline uint2x4 make_uint2x4(u32 x1, u32 x2, u32 x3, u32 x4, u32 x5, u32 x6, u32 x7, u32 x8)
{
    uint2x4 result = {{{{x1, x2, x3, x4}}, {{x5, x6, x7, x8}}}};
    return result;
}

typedef _Kernel::array<u32, 3, 1>         uint3;
static inline uint3 make_uint3(u32 x1, u32 x2, u32 x3)
{
    uint3 result = {{x1, x2, x3}};
    return result;
}
typedef _Kernel::array<u32, 3, 2>         uint3x2;
static inline uint3x2 make_uint3x2(u32 x1, u32 x2, u32 x3, u32 x4, u32 x5, u32 x6)
{
    uint3x2 result = {{{{x1, x2}}, {{x3, x4}}, {{x5, x6}}}};
    return result;
}
typedef _Kernel::array<u32, 3, 3>         uint3x3;
static inline uint3x3 make_uint3x3(u32 x1, u32 x2, u32 x3, u32 x4, u32 x5, u32 x6, u32 x7, u32 x8, u32 x9)
{
    uint3x3 result = {{{{x1, x2, x3}}, {{x4, x5, x6}}, {{x7, x8, x9}}}};
    return result;
}
typedef _Kernel::array<u32, 3, 4>         uint3x4;
static inline uint3x4 make_uint3x4(u32 x1, u32 x2, u32 x3, u32 x4, u32 x5, u32 x6, u32 x7, u32 x8, u32 x9, u32 x10, u32 x11, u32 x12)
{
    uint3x4 result = {{{{x1, x2, x3, x4}}, {{x5, x6, x7, x8}}, {{x9, x10, x11, x12}}}};
    return result;
}

typedef _Kernel::array<u32, 4, 1>         uint4;
static inline uint4 make_uint4(u32 x1, u32 x2, u32 x3, u32 x4)
{
    uint4 result = {{x1, x2, x3, x4}};
    return result;
}
typedef _Kernel::array<u32, 4, 2>         uint4x2;
static inline uint4x2 make_uint4x2(u32 x1, u32 x2, u32 x3, u32 x4, u32 x5, u32 x6, u32 x7, u32 x8)
{
    uint4x2 result = {{{{x1, x2}}, {{x3, x4}}, {{x5, x6}}, {{x7, x8}}}};
    return result;
}
typedef _Kernel::array<u32, 4, 3>         uint4x3;
static inline uint4x3 make_uint4x3(u32 x1, u32 x2, u32 x3, u32 x4, u32 x5, u32 x6, u32 x7, u32 x8, u32 x9, u32 x10, u32 x11, u32 x12)
{
    uint4x3 result = {{{{x1, x2, x3}}, {{x4, x5, x6}}, {{x7, x8, x9}}, {{x10, x11, x12}}}};
    return result;
}
typedef _Kernel::array<u32, 4, 4>         uint4x4;
static inline uint4x4 make_uint4x4(u32 x1, u32 x2, u32 x3, u32 x4, u32 x5, u32 x6, u32 x7, u32 x8, u32 x9, u32 x10, u32 x11, u32 x12, u32 x13, u32 x14, u32 x15, u32 x16)
{
    uint4x4 result = {{{{x1, x2, x3, x4}}, {{x5, x6, x7, x8}}, {{x9, x10, x11, x12}}, {{x13, x14, x15, x16}}}};
    return result;
}


typedef _Kernel::array<i64, 1, 2>         bigint1x2;
static inline bigint1x2 make_bigint1x2(i64 x1, i64 x2)
{
    bigint1x2 result = {{x1, x2}};
    return result;
}
typedef _Kernel::array<i64, 1, 3>         bigint1x3;
static inline bigint1x3 make_bigint1x3(i64 x1, i64 x2, i64 x3)
{
    bigint1x3 result = {{x1, x2, x3}};
    return result;
}
typedef _Kernel::array<i64, 1, 4>         bigint1x4;
static inline bigint1x4 make_bigint1x4(i64 x1, i64 x2, i64 x3, i64 x4)
{
    bigint1x4 result = {{x1, x2, x3, x4}};
    return result;
}

typedef _Kernel::array<i64, 2, 1>         bigint2;
static inline bigint2 make_bigint2(i64 x1, i64 x2)
{
    bigint2 result = {{x1, x2}};
    return result;
}
typedef _Kernel::array<i64, 2, 2>         bigint2x2;
static inline bigint2x2 make_bigint2x2(i64 x1, i64 x2, i64 x3, i64 x4)
{
    bigint2x2 result = {{{{x1, x2}}, {{x3, x4}}}};
    return result;
}
typedef _Kernel::array<i64, 2, 3>         bigint2x3;
static inline bigint2x3 make_bigint2x3(i64 x1, i64 x2, i64 x3, i64 x4, i64 x5, i64 x6)
{
    bigint2x3 result = {{{{x1, x2, x3}}, {{x4, x5, x6}}}};
    return result;
}
typedef _Kernel::array<i64, 2, 4>         bigint2x4;
static inline bigint2x4 make_bigint2x4(i64 x1, i64 x2, i64 x3, i64 x4, i64 x5, i64 x6, i64 x7, i64 x8)
{
    bigint2x4 result = {{{{x1, x2, x3, x4}}, {{x5, x6, x7, x8}}}};
    return result;
}

typedef _Kernel::array<i64, 3, 1>         bigint3;
static inline bigint3 make_bigint3(i64 x1, i64 x2, i64 x3)
{
    bigint3 result = {{x1, x2, x3}};
    return result;
}
typedef _Kernel::array<i64, 3, 2>         bigint3x2;
static inline bigint3x2 make_bigint3x2(i64 x1, i64 x2, i64 x3, i64 x4, i64 x5, i64 x6)
{
    bigint3x2 result = {{{{x1, x2}}, {{x3, x4}}, {{x5, x6}}}};
    return result;
}
typedef _Kernel::array<i64, 3, 3>         bigint3x3;
static inline bigint3x3 make_bigint3x3(i64 x1, i64 x2, i64 x3, i64 x4, i64 x5, i64 x6, i64 x7, i64 x8, i64 x9)
{
    bigint3x3 result = {{{{x1, x2, x3}}, {{x4, x5, x6}}, {{x7, x8, x9}}}};
    return result;
}
typedef _Kernel::array<i64, 3, 4>         bigint3x4;
static inline bigint3x4 make_bigint3x4(i64 x1, i64 x2, i64 x3, i64 x4, i64 x5, i64 x6, i64 x7, i64 x8, i64 x9, i64 x10, i64 x11, i64 x12)
{
    bigint3x4 result = {{{{x1, x2, x3, x4}}, {{x5, x6, x7, x8}}, {{x9, x10, x11, x12}}}};
    return result;
}

typedef _Kernel::array<i64, 4, 1>         bigint4;
static inline bigint4 make_bigint4(i64 x1, i64 x2, i64 x3, i64 x4)
{
    bigint4 result = {{x1, x2, x3, x4}};
    return result;
}
typedef _Kernel::array<i64, 4, 2>         bigint4x2;
static inline bigint4x2 make_bigint4x2(i64 x1, i64 x2, i64 x3, i64 x4, i64 x5, i64 x6, i64 x7, i64 x8)
{
    bigint4x2 result = {{{{x1, x2}}, {{x3, x4}}, {{x5, x6}}, {{x7, x8}}}};
    return result;
}
typedef _Kernel::array<i64, 4, 3>         bigint4x3;
static inline bigint4x3 make_bigint4x3(i64 x1, i64 x2, i64 x3, i64 x4, i64 x5, i64 x6, i64 x7, i64 x8, i64 x9, i64 x10, i64 x11, i64 x12)
{
    bigint4x3 result = {{{{x1, x2, x3}}, {{x4, x5, x6}}, {{x7, x8, x9}}, {{x10, x11, x12}}}};
    return result;
}
typedef _Kernel::array<i64, 4, 4>         bigint4x4;
static inline bigint4x4 make_bigint4x4(i64 x1, i64 x2, i64 x3, i64 x4, i64 x5, i64 x6, i64 x7, i64 x8, i64 x9, i64 x10, i64 x11, i64 x12, i64 x13, i64 x14, i64 x15, i64 x16)
{
    bigint4x4 result = {{{{x1, x2, x3, x4}}, {{x5, x6, x7, x8}}, {{x9, x10, x11, x12}}, {{x13, x14, x15, x16}}}};
    return result;
}


typedef _Kernel::array<u64, 1, 2>         biguint1x2;
static inline biguint1x2 make_biguint1x2(u64 x1, u64 x2)
{
    biguint1x2 result = {{x1, x2}};
    return result;
}
typedef _Kernel::array<u64, 1, 3>         biguint1x3;
static inline biguint1x3 make_biguint1x3(u64 x1, u64 x2, u64 x3)
{
    biguint1x3 result = {{x1, x2, x3}};
    return result;
}
typedef _Kernel::array<u64, 1, 4>         biguint1x4;
static inline biguint1x4 make_biguint1x4(u64 x1, u64 x2, u64 x3, u64 x4)
{
    biguint1x4 result = {{x1, x2, x3, x4}};
    return result;
}

typedef _Kernel::array<u64, 2, 1>         biguint2;
static inline biguint2 make_biguint2(u64 x1, u64 x2)
{
    biguint2 result = {{x1, x2}};
    return result;
}
typedef _Kernel::array<u64, 2, 2>         biguint2x2;
static inline biguint2x2 make_biguint2x2(u64 x1, u64 x2, u64 x3, u64 x4)
{
    biguint2x2 result = {{{{x1, x2}}, {{x3, x4}}}};
    return result;
}
typedef _Kernel::array<u64, 2, 3>         biguint2x3;
static inline biguint2x3 make_biguint2x3(u64 x1, u64 x2, u64 x3, u64 x4, u64 x5, u64 x6)
{
    biguint2x3 result = {{{{x1, x2, x3}}, {{x4, x5, x6}}}};
    return result;
}
typedef _Kernel::array<u64, 2, 4>         biguint2x4;
static inline biguint2x4 make_biguint2x4(u64 x1, u64 x2, u64 x3, u64 x4, u64 x5, u64 x6, u64 x7, u64 x8)
{
    biguint2x4 result = {{{{x1, x2, x3, x4}}, {{x5, x6, x7, x8}}}};
    return result;
}

typedef _Kernel::array<u64, 3, 1>         biguint3;
static inline biguint3 make_biguint3(u64 x1, u64 x2, u64 x3)
{
    biguint3 result = {{x1, x2, x3}};
    return result;
}
typedef _Kernel::array<u64, 3, 2>         biguint3x2;
static inline biguint3x2 make_biguint3x2(u64 x1, u64 x2, u64 x3, u64 x4, u64 x5, u64 x6)
{
    biguint3x2 result = {{{{x1, x2}}, {{x3, x4}}, {{x5, x6}}}};
    return result;
}
typedef _Kernel::array<u64, 3, 3>         biguint3x3;
static inline biguint3x3 make_biguint3x3(u64 x1, u64 x2, u64 x3, u64 x4, u64 x5, u64 x6, u64 x7, u64 x8, u64 x9)
{
    biguint3x3 result = {{{{x1, x2, x3}}, {{x4, x5, x6}}, {{x7, x8, x9}}}};
    return result;
}
typedef _Kernel::array<u64, 3, 4>         biguint3x4;
static inline biguint3x4 make_biguint3x4(u64 x1, u64 x2, u64 x3, u64 x4, u64 x5, u64 x6, u64 x7, u64 x8, u64 x9, u64 x10, u64 x11, u64 x12)
{
    biguint3x4 result = {{{{x1, x2, x3, x4}}, {{x5, x6, x7, x8}}, {{x9, x10, x11, x12}}}};
    return result;
}

typedef _Kernel::array<u64, 4, 1>         biguint4;
static inline biguint4 make_biguint4(u64 x1, u64 x2, u64 x3, u64 x4)
{
    biguint4 result = {{x1, x2, x3, x4}};
    return result;
}
typedef _Kernel::array<u64, 4, 2>         biguint4x2;
static inline biguint4x2 make_biguint4x2(u64 x1, u64 x2, u64 x3, u64 x4, u64 x5, u64 x6, u64 x7, u64 x8)
{
    biguint4x2 result = {{{{x1, x2}}, {{x3, x4}}, {{x5, x6}}, {{x7, x8}}}};
    return result;
}
typedef _Kernel::array<u64, 4, 3>         biguint4x3;
static inline biguint4x3 make_biguint4x3(u64 x1, u64 x2, u64 x3, u64 x4, u64 x5, u64 x6, u64 x7, u64 x8, u64 x9, u64 x10, u64 x11, u64 x12)
{
    biguint4x3 result = {{{{x1, x2, x3}}, {{x4, x5, x6}}, {{x7, x8, x9}}, {{x10, x11, x12}}}};
    return result;
}
typedef _Kernel::array<u64, 4, 4>         biguint4x4;
static inline biguint4x4 make_biguint4x4(u64 x1, u64 x2, u64 x3, u64 x4, u64 x5, u64 x6, u64 x7, u64 x8, u64 x9, u64 x10, u64 x11, u64 x12, u64 x13, u64 x14, u64 x15, u64 x16)
{
    biguint4x4 result = {{{{x1, x2, x3, x4}}, {{x5, x6, x7, x8}}, {{x9, x10, x11, x12}}, {{x13, x14, x15, x16}}}};
    return result;
}


typedef _Kernel::array<float, 1, 2>       float1x2;
static inline float1x2 make_float1x2(float x1, float x2)
{
    float1x2 result = {{x1, x2}};
    return result;
}
typedef _Kernel::array<float, 1, 3>       float1x3;
static inline float1x3 make_float1x3(float x1, float x2, float x3)
{
    float1x3 result = {{x1, x2, x3}};
    return result;
}
typedef _Kernel::array<float, 1, 4>       float1x4;
static inline float1x4 make_float1x4(float x1, float x2, float x3, float x4)
{
    float1x4 result = {{x1, x2, x3, x4}};
    return result;
}

typedef _Kernel::array<float, 2, 1>       float2;
static inline float2 make_float2(float x1, float x2)
{
    float2 result = {{x1, x2}};
    return result;
}
typedef _Kernel::array<float, 2, 2>       float2x2;
static inline float2x2 make_float2x2(float x1, float x2, float x3, float x4)
{
    float2x2 result = {{{{x1, x2}}, {{x3, x4}}}};
    return result;
}
typedef _Kernel::array<float, 2, 3>       float2x3;
static inline float2x3 make_float2x3(float x1, float x2, float x3, float x4, float x5, float x6)
{
    float2x3 result = {{{{x1, x2, x3}}, {{x4, x5, x6}}}};
    return result;
}
typedef _Kernel::array<float, 2, 4>       float2x4;
static inline float2x4 make_float2x4(float x1, float x2, float x3, float x4, float x5, float x6, float x7, float x8)
{
    float2x4 result = {{{{x1, x2, x3, x4}}, {{x5, x6, x7, x8}}}};
    return result;
}

typedef _Kernel::array<float, 3, 1>       float3;
static inline float3 make_float3(float x1, float x2, float x3)
{
    float3 result = {{x1, x2, x3}};
    return result;
}
typedef _Kernel::array<float, 3, 2>       float3x2;
static inline float3x2 make_float3x2(float x1, float x2, float x3, float x4, float x5, float x6)
{
    float3x2 result = {{{{x1, x2}}, {{x3, x4}}, {{x5, x6}}}};
    return result;
}
typedef _Kernel::array<float, 3, 3>       float3x3;
static inline float3x3 make_float3x3(float x1, float x2, float x3, float x4, float x5, float x6, float x7, float x8, float x9)
{
    float3x3 result = {{{{x1, x2, x3}}, {{x4, x5, x6}}, {{x7, x8, x9}}}};
    return result;
}
typedef _Kernel::array<float, 3, 4>       float3x4;
static inline float3x4 make_float3x4(float x1, float x2, float x3, float x4, float x5, float x6, float x7, float x8, float x9, float x10, float x11, float x12)
{
    float3x4 result = {{{{x1, x2, x3, x4}}, {{x5, x6, x7, x8}}, {{x9, x10, x11, x12}}}};
    return result;
}

typedef _Kernel::array<float, 4, 1>       float4;
static inline float4 make_float4(float x1, float x2, float x3, float x4)
{
    float4 result = {{x1, x2, x3, x4}};
    return result;
}
typedef _Kernel::array<float, 4, 2>       float4x2;
static inline float4x2 make_float4x2(float x1, float x2, float x3, float x4, float x5, float x6, float x7, float x8)
{
    float4x2 result = {{{{x1, x2}}, {{x3, x4}}, {{x5, x6}}, {{x7, x8}}}};
    return result;
}
typedef _Kernel::array<float, 4, 3>       float4x3;
static inline float4x3 make_float4x3(float x1, float x2, float x3, float x4, float x5, float x6, float x7, float x8, float x9, float x10, float x11, float x12)
{
    float4x3 result = {{{{x1, x2, x3}}, {{x4, x5, x6}}, {{x7, x8, x9}}, {{x10, x11, x12}}}};
    return result;
}
typedef _Kernel::array<float, 4, 4>       float4x4;
static inline float4x4 make_float4x4(float x1, float x2, float x3, float x4, float x5, float x6, float x7, float x8, float x9, float x10, float x11, float x12, float x13, float x14, float x15, float x16)
{
    float4x4 result = {{{{x1, x2, x3, x4}}, {{x5, x6, x7, x8}}, {{x9, x10, x11, x12}}, {{x13, x14, x15, x16}}}};
    return result;
}



typedef _Kernel::array<u8, 4, 1>          color32;
typedef _Kernel::array<u8, 4, 1>          color128;

/*****************************************************************************/
#endif
