#include    <rtti/stdafx.h>
#include    <rtti/classinfo.script.hh>

#if 0

builtin bool
{
};

builtin u8
{
};

builtin u16
{
};

builtin u32
{
};

builtin u64
{
};

builtin i8
{
};

builtin i16
{
};

builtin i32
{
};

builtin i64
{
};

builtin float
{
};

builtin double
{
};

namespace BugEngine
{
struct istring
{
};

struct inamespace
{
};

struct ifilename
{
};

struct ipath
{
};

namespace RTTI
{
struct Value
{
};
}

struct float2
{
published:
    float2();
    float2(float x, float y);
};

struct float3
{
    float3();
    float3(float x, float y, float z);
};

struct float4
{
    float4();
    float4(float x, float y, float z, float w);
};


struct int2
{
published:
    int2();
    int2(i32 x, i32 y);
};

struct int3
{
    int3();
    int3(i32 x, i32 y, i32 z);
};

struct int4
{
    int4();
    int4(i32 x, i32 y, i32 z, i32 w);
};

struct uint2
{
published:
    uint2();
    uint2(u32 x, u32 y);
};

struct uint3
{
    uint3();
    uint3(u32 x, u32 y, u32 z);
};

struct uint4
{
    uint4();
    uint4(u32 x, u32 y, u32 z, u32 w);
};

}


#endif
