/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_KERNEL_SIMD_HH_
#define BE_KERNEL_SIMD_HH_
/**************************************************************************************************/
#include    <kernel/stdafx.h>

namespace _Kernel
{

template< typename T, unsigned C >
struct array
{
    T   t[C];
    T& operator[](u32 index)
    {
        return t[index];
    }
    T operator[](u32 index) const
    {
        return t[index];
    }
};

}

namespace kernel
{

typedef _Kernel::array<u8, 4>       color32;
typedef _Kernel::array<float, 4>    color128;


typedef _Kernel::array<u8, 2> ubyte2;
typedef _Kernel::array<u8, 3> ubyte3;
typedef _Kernel::array<u8, 4> ubyte4;
typedef _Kernel::array<u8, 8> ubyte8;
typedef _Kernel::array<u8, 16> ubyte16;

static inline ubyte2 make_ubyte2(u8 x1, u8 x2)
{
    ubyte2 result = {{x1, x2}};
    return result;
}
static inline ubyte3 make_ubyte3(u8 x1, u8 x2, u8 x3)
{
    ubyte3 result = {{x1, x2, x3}};
    return result;
}
static inline ubyte4 make_ubyte4(u8 x1, u8 x2, u8 x3, u8 x4)
{
    ubyte4 result = {{x1, x2, x3, x4}};
    return result;
}
static inline ubyte8 make_ubyte8(u8 x1, u8 x2, u8 x3, u8 x4,
                                 u8 x5, u8 x6, u8 x7, u8 x8)
{
    ubyte8 result = {{x1, x2, x3, x4, x5, x6, x7, x8}};
    return result;
}
static inline ubyte16 make_ubyte16(u8 x1, u8 x2, u8 x3, u8 x4,
                                   u8 x5, u8 x6, u8 x7, u8 x8,
                                   u8 x9, u8 x10, u8 x11, u8 x12,
                                   u8 x13, u8 x14, u8 x15, u8 x16)
{
    ubyte16 result = {{x1, x2, x3, x4, x5, x6, x7, x8,
                       x9, x10, x11, x12, x13, x14, x15, x16}};
    return result;
}


typedef _Kernel::array<i8, 2> byte2;
typedef _Kernel::array<i8, 3> byte3;
typedef _Kernel::array<i8, 4> byte4;
typedef _Kernel::array<i8, 8> byte8;
typedef _Kernel::array<i8, 16> byte16;

static inline byte2 make_byte2(i8 x1, i8 x2)
{
    byte2 result = {{x1, x2}};
    return result;
}
static inline byte3 make_byte3(i8 x1, i8 x2, i8 x3)
{
    byte3 result = {{x1, x2, x3}};
    return result;
}
static inline byte4 make_byte4(i8 x1, i8 x2, i8 x3, i8 x4)
{
    byte4 result = {{x1, x2, x3, x4}};
    return result;
}
static inline byte8 make_byte8(i8 x1, i8 x2, i8 x3, i8 x4,
                               i8 x5, i8 x6, i8 x7, i8 x8)
{
    byte8 result = {{x1, x2, x3, x4, x5, x6, x7, x8}};
    return result;
}
static inline byte16 make_byte16(i8 x1, i8 x2, i8 x3, i8 x4,
                                 i8 x5, i8 x6, i8 x7, i8 x8,
                                 i8 x9, i8 x10, i8 x11, i8 x12,
                                 i8 x13, i8 x14, i8 x15, i8 x16)
{
    byte16 result = {{x1, x2, x3, x4, x5, x6, x7, x8,
                      x9, x10, x11, x12, x13, x14, x15, x16}};
    return result;
}


typedef _Kernel::array<u16, 2> ushort2;
typedef _Kernel::array<u16, 3> ushort3;
typedef _Kernel::array<u16, 4> ushort4;
typedef _Kernel::array<u16, 8> ushort8;
typedef _Kernel::array<u16, 16> ushort16;

static inline ushort2 make_ushort2(u16 x1, u16 x2)
{
    ushort2 result = {{x1, x2}};
    return result;
}
static inline ushort3 make_ushort3(u16 x1, u16 x2, u16 x3)
{
    ushort3 result = {{x1, x2, x3}};
    return result;
}
static inline ushort4 make_ushort4(u16 x1, u16 x2, u16 x3, u16 x4)
{
    ushort4 result = {{x1, x2, x3, x4}};
    return result;
}
static inline ushort8 make_ushort8(u16 x1, u16 x2, u16 x3, u16 x4,
                                   u16 x5, u16 x6, u16 x7, u16 x8)
{
    ushort8 result = {{x1, x2, x3, x4, x5, x6, x7, x8}};
    return result;
}
static inline ushort16 make_ushort16(u16 x1, u16 x2, u16 x3, u16 x4,
                                     u16 x5, u16 x6, u16 x7, u16 x8,
                                     u16 x9, u16 x10, u16 x11, u16 x12,
                                     u16 x13, u16 x14, u16 x15, u16 x16)
{
    ushort16 result = {{x1, x2, x3, x4, x5, x6, x7, x8,
                        x9, x10, x11, x12, x13, x14, x15, x16}};
    return result;
}


typedef _Kernel::array<i16, 2> short2;
typedef _Kernel::array<i16, 3> short3;
typedef _Kernel::array<i16, 4> short4;
typedef _Kernel::array<i16, 8> short8;
typedef _Kernel::array<i16, 16> short16;

static inline short2 make_short2(i16 x1, i16 x2)
{
    short2 result = {{x1, x2}};
    return result;
}
static inline short3 make_short3(i16 x1, i16 x2, i16 x3)
{
    short3 result = {{x1, x2, x3}};
    return result;
}
static inline short4 make_short4(i16 x1, i16 x2, i16 x3, i16 x4)
{
    short4 result = {{x1, x2, x3, x4}};
    return result;
}
static inline short8 make_short8(i16 x1, i16 x2, i16 x3, i16 x4,
                                 i16 x5, i16 x6, i16 x7, i16 x8)
{
    short8 result = {{x1, x2, x3, x4, x5, x6, x7, x8}};
    return result;
}
static inline short16 make_short16(i16 x1, i16 x2, i16 x3, i16 x4,
                                   i16 x5, i16 x6, i16 x7, i16 x8,
                                   i16 x9, i16 x10, i16 x11, i16 x12,
                                   i16 x13, i16 x14, i16 x15, i16 x16)
{
    short16 result = {{x1, x2, x3, x4, x5, x6, x7, x8,
                       x9, x10, x11, x12, x13, x14, x15, x16}};
    return result;
}


typedef _Kernel::array<u32, 2> uint2;
typedef _Kernel::array<u32, 3> uint3;
typedef _Kernel::array<u32, 4> uint4;
typedef _Kernel::array<u32, 8> uint8;
typedef _Kernel::array<u32, 16> uint16;

static inline uint2 make_uint2(u32 x1, u32 x2)
{
    uint2 result = {{x1, x2}};
    return result;
}
static inline uint3 make_uint3(u32 x1, u32 x2, u32 x3)
{
    uint3 result = {{x1, x2, x3}};
    return result;
}
static inline uint4 make_uint4(u32 x1, u32 x2, u32 x3, u32 x4)
{
    uint4 result = {{x1, x2, x3, x4}};
    return result;
}
static inline uint8 make_uint8(u32 x1, u32 x2, u32 x3, u32 x4,
                               u32 x5, u32 x6, u32 x7, u32 x8)
{
    uint8 result = {{x1, x2, x3, x4, x5, x6, x7, x8}};
    return result;
}
static inline uint16 make_uint16(u32 x1, u32 x2, u32 x3, u32 x4,
                                 u32 x5, u32 x6, u32 x7, u32 x8,
                                 u32 x9, u32 x10, u32 x11, u32 x12,
                                 u32 x13, u32 x14, u32 x15, u32 x16)
{
    uint16 result = {{x1, x2, x3, x4, x5, x6, x7, x8,
                      x9, x10, x11, x12, x13, x14, x15, x16}};
    return result;
}


typedef _Kernel::array<i32, 2> int2;
typedef _Kernel::array<i32, 3> int3;
typedef _Kernel::array<i32, 4> int4;
typedef _Kernel::array<i32, 8> int8;
typedef _Kernel::array<i32, 16> int16;

static inline int2 make_int2(i32 x1, i32 x2)
{
    int2 result = {{x1, x2}};
    return result;
}
static inline int3 make_int3(i32 x1, i32 x2, i32 x3)
{
    int3 result = {{x1, x2, x3}};
    return result;
}
static inline int4 make_int4(i32 x1, i32 x2, i32 x3, i32 x4)
{
    int4 result = {{x1, x2, x3, x4}};
    return result;
}
static inline int8 make_int8(i32 x1, i32 x2, i32 x3, i32 x4,
                               i32 x5, i32 x6, i32 x7, i32 x8)
{
    int8 result = {{x1, x2, x3, x4, x5, x6, x7, x8}};
    return result;
}
static inline int16 make_int16(i32 x1, i32 x2, i32 x3, i32 x4,
                               i32 x5, i32 x6, i32 x7, i32 x8,
                               i32 x9, i32 x10, i32 x11, i32 x12,
                               i32 x13, i32 x14, i32 x15, i32 x16)
{
    int16 result = {{x1, x2, x3, x4, x5, x6, x7, x8,
                     x9, x10, x11, x12, x13, x14, x15, x16}};
    return result;
}


typedef _Kernel::array<u64, 2> biguint2;
typedef _Kernel::array<u64, 3> biguint3;
typedef _Kernel::array<u64, 4> biguint4;
typedef _Kernel::array<u64, 8> biguint8;
typedef _Kernel::array<u64, 16> biguint16;

static inline biguint2 make_biguint2(u64 x1, u64 x2)
{
    biguint2 result = {{x1, x2}};
    return result;
}
static inline biguint3 make_biguint3(u64 x1, u64 x2, u64 x3)
{
    biguint3 result = {{x1, x2, x3}};
    return result;
}
static inline biguint4 make_biguint4(u64 x1, u64 x2, u64 x3, u64 x4)
{
    biguint4 result = {{x1, x2, x3, x4}};
    return result;
}
static inline biguint8 make_biguint8(u64 x1, u64 x2, u64 x3, u64 x4,
                                     u64 x5, u64 x6, u64 x7, u64 x8)
{
    biguint8 result = {{x1, x2, x3, x4, x5, x6, x7, x8}};
    return result;
}
static inline biguint16 make_biguint16(u64 x1, u64 x2, u64 x3, u64 x4,
                                       u64 x5, u64 x6, u64 x7, u64 x8,
                                       u64 x9, u64 x10, u64 x11, u64 x12,
                                       u64 x13, u64 x14, u64 x15, u64 x16)
{
    biguint16 result = {{x1, x2, x3, x4, x5, x6, x7, x8,
                         x9, x10, x11, x12, x13, x14, x15, x16}};
    return result;
}


typedef _Kernel::array<i64, 2> bigint2;
typedef _Kernel::array<i64, 3> bigint3;
typedef _Kernel::array<i64, 4> bigint4;
typedef _Kernel::array<i64, 8> bigint8;
typedef _Kernel::array<i64, 16> bigint16;

static inline bigint2 make_bigint2(i64 x1, i64 x2)
{
    bigint2 result = {{x1, x2}};
    return result;
}
static inline bigint3 make_bigint3(i64 x1, i64 x2, i64 x3)
{
    bigint3 result = {{x1, x2, x3}};
    return result;
}
static inline bigint4 make_bigint4(i64 x1, i64 x2, i64 x3, i64 x4)
{
    bigint4 result = {{x1, x2, x3, x4}};
    return result;
}
static inline bigint8 make_bigint8(i64 x1, i64 x2, i64 x3, i64 x4,
                                   i64 x5, i64 x6, i64 x7, i64 x8)
{
    bigint8 result = {{x1, x2, x3, x4, x5, x6, x7, x8}};
    return result;
}
static inline bigint16 make_bigint16(i64 x1, i64 x2, i64 x3, i64 x4,
                                     i64 x5, i64 x6, i64 x7, i64 x8,
                                     i64 x9, i64 x10, i64 x11, i64 x12,
                                     i64 x13, i64 x14, i64 x15, i64 x16)
{
    bigint16 result = {{x1, x2, x3, x4, x5, x6, x7, x8,
                        x9, x10, x11, x12, x13, x14, x15, x16}};
    return result;
}


typedef _Kernel::array<float, 2> float2;
typedef _Kernel::array<float, 3> float3;
typedef _Kernel::array<float, 4> float4;
typedef _Kernel::array<float, 8> float8;
typedef _Kernel::array<float, 16> float16;

static inline float2 make_float2(float x1, float x2)
{
    float2 result = {{x1, x2}};
    return result;
}
static inline float3 make_float3(float x1, float x2, float x3)
{
    float3 result = {{x1, x2, x3}};
    return result;
}
static inline float4 make_float4(float x1, float x2, float x3, float x4)
{
    float4 result = {{x1, x2, x3, x4}};
    return result;
}
static inline float8 make_float8(float x1, float x2, float x3, float x4,
                                 float x5, float x6, float x7, float x8)
{
    float8 result = {{x1, x2, x3, x4, x5, x6, x7, x8}};
    return result;
}
static inline float16 make_float16(float x1, float x2, float x3, float x4,
                                   float x5, float x6, float x7, float x8,
                                   float x9, float x10, float x11, float x12,
                                   float x13, float x14, float x15, float x16)
{
    float16 result = {{x1, x2, x3, x4, x5, x6, x7, x8,
                       x9, x10, x11, x12, x13, x14, x15, x16}};
    return result;
}


typedef _Kernel::array<double, 2> double2;
typedef _Kernel::array<double, 3> double3;
typedef _Kernel::array<double, 4> double4;
typedef _Kernel::array<double, 8> double8;
typedef _Kernel::array<double, 16> double16;

static inline double2 make_double2(double x1, double x2)
{
    double2 result = {{x1, x2}};
    return result;
}

static inline double3 make_double3(double x1, double x2, double x3)
{
    double3 result = {{x1, x2, x3}};
    return result;
}

static inline double4 make_double4(double x1, double x2, double x3, double x4)
{
    double4 result = {{x1, x2, x3, x4}};
    return result;
}

static inline double8 make_double8(double x1, double x2, double x3, double x4,
                                   double x5, double x6, double x7, double x8)
{
    double8 result = {{x1, x2, x3, x4, x5, x6, x7, x8}};
    return result;
}

static inline double16 make_double16(double x1, double x2, double x3, double x4,
                                     double x5, double x6, double x7, double x8,
                                     double x9, double x10, double x11, double x12,
                                     double x13, double x14, double x15, double x16)
{
    double16 result = {{x1, x2, x3, x4, x5, x6, x7, x8,
                        x9, x10, x11, x12, x13, x14, x15, x16}};
    return result;
}

}

/**************************************************************************************************/
#endif
