/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/classinfo.script.hh>

#if 0

namespace BugEngine
{

be_tag(Index(BugEngine::RTTI::ClassType_String + (0 << 16)))
struct istring
{
};

be_tag(Index(BugEngine::RTTI::ClassType_String + (1 << 16)))
struct inamespace
{
};

be_tag(Index(BugEngine::RTTI::ClassType_String + (2 << 16)))
struct ifilename
{
};

be_tag(Index(BugEngine::RTTI::ClassType_String + (3 << 16)))
struct ipath
{
};

}

namespace BugEngine { namespace RTTI
{

be_tag(Index(BugEngine::RTTI::ClassType_Variant))
struct Value
{
};

}}


be_tag(Index(BugEngine::RTTI::ClassType_Integer))
be_pod bool
{
};

be_tag(Index(BugEngine::RTTI::ClassType_Integer + (1 << 16)))
be_pod u8
{
};

be_tag(Index(BugEngine::RTTI::ClassType_Integer + (2 << 16)))
be_pod u16
{
};

be_tag(Index(BugEngine::RTTI::ClassType_Integer + (3 << 16)))
be_pod u32
{
};

be_tag(Index(BugEngine::RTTI::ClassType_Integer + (4 << 16)))
be_pod u64
{
};


be_tag(Index(BugEngine::RTTI::ClassType_Integer + (5 << 16)))
be_pod i8
{
};

be_tag(Index(BugEngine::RTTI::ClassType_Integer + (6 << 16)))
be_pod i16
{
};

be_tag(Index(BugEngine::RTTI::ClassType_Integer + (7 << 16)))
be_pod i32
{
};

be_tag(Index(BugEngine::RTTI::ClassType_Integer + (8 << 16)))
be_pod i64
{
};

be_tag(Index(BugEngine::RTTI::ClassType_Integer + (9 << 16)))
be_pod float
{
};

be_tag(Index(BugEngine::RTTI::ClassType_Integer + (10 << 16)))
be_pod double
{
};

be_pod byte1x2
{
    u8 operator[](u32) const;
    u8& operator[](u32);
};
be_pod byte1x3
{
    u8 operator[](u32) const;
    u8& operator[](u32);
};
be_pod byte1x4
{
    u8 operator[](u32) const;
    u8& operator[](u32);
};

be_pod byte2
{
    u8 operator[](u32) const;
    u8& operator[](u32);
};
be_pod byte2x2
{
    const byte2& operator[](u32) const;
    byte2& operator[](u32);
};
be_pod byte2x3
{
    const byte2& operator[](u32) const;
    byte2& operator[](u32);
};
be_pod byte2x4
{
    const byte2& operator[](u32) const;
    byte2& operator[](u32);
};

be_pod byte3
{
    u8 operator[](u32) const;
    u8& operator[](u32);
};
be_pod byte3x2
{
    const byte3& operator[](u32) const;
    byte3& operator[](u32);
};
be_pod byte3x3
{
    const byte3& operator[](u32) const;
    byte3& operator[](u32);
};
be_pod byte3x4
{
    const byte3& operator[](u32) const;
    byte3& operator[](u32);
};

be_pod byte4
{
    u8 operator[](u32) const;
    u8& operator[](u32);
};
be_pod byte4x2
{
    const byte4& operator[](u32) const;
    byte4& operator[](u32);
};
be_pod byte4x3
{
    const byte4& operator[](u32) const;
    byte4& operator[](u32);
};
be_pod byte4x4
{
    const byte4& operator[](u32) const;
    byte4& operator[](u32);
};


be_pod short1x2
{
    i16 operator[](u32) const;
    i16& operator[](u32);
};
be_pod short1x3
{
    i16 operator[](u32) const;
    i16& operator[](u32);
};
be_pod short1x4
{
    i16 operator[](u32) const;
    i16& operator[](u32);
};

be_pod short2
{
    i16 operator[](u32) const;
    i16& operator[](u32);
};
be_pod short2x2
{
    const short2& operator[](u32) const;
    short2& operator[](u32);
};
be_pod short2x3
{
    const short2& operator[](u32) const;
    short2& operator[](u32);
};
be_pod short2x4
{
    const short2& operator[](u32) const;
    short2& operator[](u32);
};

be_pod short3
{
    i16 operator[](u32) const;
    i16& operator[](u32);
};
be_pod short3x2
{
    const short3& operator[](u32) const;
    short3& operator[](u32);
};
be_pod short3x3
{
    const short3& operator[](u32) const;
    short3& operator[](u32);
};
be_pod short3x4
{
    const short3& operator[](u32) const;
    short3& operator[](u32);
};

be_pod short4
{
    i16 operator[](u32) const;
    i16& operator[](u32);
};
be_pod short4x2
{
    const short4& operator[](u32) const;
    short4& operator[](u32);
};
be_pod short4x3
{
    const short4& operator[](u32) const;
    short4& operator[](u32);
};
be_pod short4x4
{
    const short4& operator[](u32) const;
    short4& operator[](u32);
};


be_pod ushort1x2
{
    u16 operator[](u32) const;
    u16& operator[](u32);
};
be_pod ushort1x3
{
    u16 operator[](u32) const;
    u16& operator[](u32);
};
be_pod ushort1x4
{
    u16 operator[](u32) const;
    u16& operator[](u32);
};

be_pod ushort2
{
    u16 operator[](u32) const;
    u16& operator[](u32);
};
be_pod ushort2x2
{
    const ushort2& operator[](u32) const;
    ushort2& operator[](u32);
};
be_pod ushort2x3
{
    const ushort2& operator[](u32) const;
    ushort2& operator[](u32);
};
be_pod ushort2x4
{
    const ushort2& operator[](u32) const;
    ushort2& operator[](u32);
};

be_pod ushort3
{
    u16 operator[](u32) const;
    u16& operator[](u32);
};
be_pod ushort3x2
{
    const ushort3& operator[](u32) const;
    ushort3& operator[](u32);
};
be_pod ushort3x3
{
    const ushort3& operator[](u32) const;
    ushort3& operator[](u32);
};
be_pod ushort3x4
{
    const ushort3& operator[](u32) const;
    ushort3& operator[](u32);
};

be_pod ushort4
{
    u16 operator[](u32) const;
    u16& operator[](u32);
};
be_pod ushort4x2
{
    const ushort4& operator[](u32) const;
    ushort4& operator[](u32);
};
be_pod ushort4x3
{
    const ushort4& operator[](u32) const;
    ushort4& operator[](u32);
};
be_pod ushort4x4
{
    const ushort4& operator[](u32) const;
    ushort4& operator[](u32);
};


be_pod int1x2
{
    i32 operator[](u32) const;
    i32& operator[](u32);
};
be_pod int1x3
{
    i32 operator[](u32) const;
    i32& operator[](u32);
};
be_pod int1x4
{
    i32 operator[](u32) const;
    i32& operator[](u32);
};

be_pod int2
{
    i32 operator[](u32) const;
    i32& operator[](u32);
};
be_pod int2x2
{
    const int2& operator[](u32) const;
    int2& operator[](u32);
};
be_pod int2x3
{
    const int2& operator[](u32) const;
    int2& operator[](u32);
};
be_pod int2x4
{
    const int2& operator[](u32) const;
    int2& operator[](u32);
};

be_pod int3
{
    i32 operator[](u32) const;
    i32& operator[](u32);
};
be_pod int3x2
{
    const int3& operator[](u32) const;
    int3& operator[](u32);
};
be_pod int3x3
{
    const int3& operator[](u32) const;
    int3& operator[](u32);
};
be_pod int3x4
{
    const int3& operator[](u32) const;
    int3& operator[](u32);
};

be_pod int4
{
    i32 operator[](u32) const;
    i32& operator[](u32);
};
be_pod int4x2
{
    const int4& operator[](u32) const;
    int4& operator[](u32);
};
be_pod int4x3
{
    const int4& operator[](u32) const;
    int4& operator[](u32);
};
be_pod int4x4
{
    const int4& operator[](u32) const;
    int4& operator[](u32);
};


be_pod uint1x2
{
    u32 operator[](u32) const;
    u32& operator[](u32);
};
be_pod uint1x3
{
    u32 operator[](u32) const;
    u32& operator[](u32);
};
be_pod uint1x4
{
    u32 operator[](u32) const;
    u32& operator[](u32);
};

be_pod uint2
{
    u32 operator[](u32) const;
    u32& operator[](u32);
};
be_pod uint2x2
{
    const uint2& operator[](u32) const;
    uint2& operator[](u32);
};
be_pod uint2x3
{
    const uint2& operator[](u32) const;
    uint2& operator[](u32);
};
be_pod uint2x4
{
    const uint2& operator[](u32) const;
    uint2& operator[](u32);
};

be_pod uint3
{
    u32 operator[](u32) const;
    u32& operator[](u32);
};
be_pod uint3x2
{
    const uint3& operator[](u32) const;
    uint3& operator[](u32);
};
be_pod uint3x3
{
    const uint3& operator[](u32) const;
    uint3& operator[](u32);
};
be_pod uint3x4
{
    const uint3& operator[](u32) const;
    uint3& operator[](u32);
};

be_pod uint4
{
    u32 operator[](u32) const;
    u32& operator[](u32);
};
be_pod uint4x2
{
    const uint4& operator[](u32) const;
    uint4& operator[](u32);
};
be_pod uint4x3
{
    const uint4& operator[](u32) const;
    uint4& operator[](u32);
};
be_pod uint4x4
{
    const uint4& operator[](u32) const;
    uint4& operator[](u32);
};


be_pod bigint1x2
{
    i64 operator[](u32) const;
    i64& operator[](u32);
};
be_pod bigint1x3
{
    i64 operator[](u32) const;
    i64& operator[](u32);
};
be_pod bigint1x4
{
    i64 operator[](u32) const;
    i64& operator[](u32);
};

be_pod bigint2
{
    i64 operator[](u32) const;
    i64& operator[](u32);
};
be_pod bigint2x2
{
    const bigint2& operator[](u32) const;
    bigint2& operator[](u32);
};
be_pod bigint2x3
{
    const bigint2& operator[](u32) const;
    bigint2& operator[](u32);
};
be_pod bigint2x4
{
    const bigint2& operator[](u32) const;
    bigint2& operator[](u32);
};

be_pod bigint3
{
    i64 operator[](u32) const;
    i64& operator[](u32);
};
be_pod bigint3x2
{
    const bigint3& operator[](u32) const;
    bigint3& operator[](u32);
};
be_pod bigint3x3
{
    const bigint3& operator[](u32) const;
    bigint3& operator[](u32);
};
be_pod bigint3x4
{
    const bigint3& operator[](u32) const;
    bigint3& operator[](u32);
};

be_pod bigint4
{
    i64 operator[](u32) const;
    i64& operator[](u32);
};
be_pod bigint4x2
{
    const bigint4& operator[](u32) const;
    bigint4& operator[](u32);
};
be_pod bigint4x3
{
    const bigint4& operator[](u32) const;
    bigint4& operator[](u32);
};
be_pod bigint4x4
{
    const bigint4& operator[](u32) const;
    bigint4& operator[](u32);
};


be_pod biguint1x2
{
    u64 operator[](u32) const;
    u64& operator[](u32);
};
be_pod biguint1x3
{
    u64 operator[](u32) const;
    u64& operator[](u32);
};
be_pod biguint1x4
{
    u64 operator[](u32) const;
    u64& operator[](u32);
};

be_pod biguint2
{
    u64 operator[](u32) const;
    u64& operator[](u32);
};
be_pod biguint2x2
{
    const biguint2& operator[](u32) const;
    biguint2& operator[](u32);
};
be_pod biguint2x3
{
    const biguint2& operator[](u32) const;
    biguint2& operator[](u32);
};
be_pod biguint2x4
{
    const biguint2& operator[](u32) const;
    biguint2& operator[](u32);
};

be_pod biguint3
{
    u64 operator[](u32) const;
    u64& operator[](u32);
};
be_pod biguint3x2
{
    const biguint3& operator[](u32) const;
    biguint3& operator[](u32);
};
be_pod biguint3x3
{
    const biguint3& operator[](u32) const;
    biguint3& operator[](u32);
};
be_pod biguint3x4
{
    const biguint3& operator[](u32) const;
    biguint3& operator[](u32);
};

be_pod biguint4
{
    u64 operator[](u32) const;
    u64& operator[](u32);
};
be_pod biguint4x2
{
    const biguint4& operator[](u32) const;
    biguint4& operator[](u32);
};
be_pod biguint4x3
{
    const biguint4& operator[](u32) const;
    biguint4& operator[](u32);
};
be_pod biguint4x4
{
    const biguint4& operator[](u32) const;
    biguint4& operator[](u32);
};


be_pod float1x2
{
    float operator[](u32) const;
    float& operator[](u32);
};
be_pod float1x3
{
    float operator[](u32) const;
    float& operator[](u32);
};
be_pod float1x4
{
    float operator[](u32) const;
    float& operator[](u32);
};

be_pod float2
{
    float operator[](u32) const;
    float& operator[](u32);
};
be_pod float2x2
{
    const float2& operator[](u32) const;
    float2& operator[](u32);
};
be_pod float2x3
{
    const float2& operator[](u32) const;
    float2& operator[](u32);
};
be_pod float2x4
{
    const float2& operator[](u32) const;
    float2& operator[](u32);
};

be_pod float3
{
    float operator[](u32) const;
    float& operator[](u32);
};
be_pod float3x2
{
    const float3& operator[](u32) const;
    float3& operator[](u32);
};
be_pod float3x3
{
    const float3& operator[](u32) const;
    float3& operator[](u32);
};
be_pod float3x4
{
    const float3& operator[](u32) const;
    float3& operator[](u32);
};

be_pod float4
{
    float operator[](u32) const;
    float& operator[](u32);
};
be_pod float4x2
{
    const float4& operator[](u32) const;
    float4& operator[](u32);
};
be_pod float4x3
{
    const float4& operator[](u32) const;
    float4& operator[](u32);
};
be_pod float4x4
{
    const float4& operator[](u32) const;
    float4& operator[](u32);
};

#endif
