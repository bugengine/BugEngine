/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_BUILTIN_SCRIPT_HH_
#define BE_RTTI_BUILTIN_SCRIPT_HH_
/**************************************************************************************************/
#include    <rtti/stdafx.h>
#include    <rtti/classinfo.script.hh>
#include    <rtti/builtin.hh>


#if 0

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

be_tag(Index(BugEngine::RTTI::ClassType_Vector2
          + (BugEngine::RTTI::ClassIndex_float << 16)))
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

be_tag(Index(BugEngine::RTTI::ClassType_Vector3
          + (BugEngine::RTTI::ClassIndex_float << 16)))
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

be_tag(Index(BugEngine::RTTI::ClassType_Vector4
          + (BugEngine::RTTI::ClassIndex_float << 16)))
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
be_tag(Index(BugEngine::RTTI::ClassType_Matrix
          + (BugEngine::RTTI::ClassIndex_float << 16)))
be_pod float4x4
{
    const float4& operator[](u32) const;
    float4& operator[](u32);
};

#endif

/**************************************************************************************************/
#endif
