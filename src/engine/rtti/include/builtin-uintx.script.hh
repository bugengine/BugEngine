/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_BUILTIN_SCRIPT_HH_
#define BE_RTTI_BUILTIN_SCRIPT_HH_
/**************************************************************************************************/
#include    <rtti/stdafx.h>
#include    <rtti/classinfo.script.hh>
#include    <rtti/builtin.hh>


#if 0

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

be_tag(Index(BugEngine::RTTI::ClassType_Vector2
          + (BugEngine::RTTI::ClassIndex_u32 << 16)))
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

be_tag(Index(BugEngine::RTTI::ClassType_Vector3
          + (BugEngine::RTTI::ClassIndex_u32 << 16)))
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

be_tag(Index(BugEngine::RTTI::ClassType_Vector4
          + (BugEngine::RTTI::ClassIndex_u32 << 16)))
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
be_tag(Index(BugEngine::RTTI::ClassType_Matrix
          + (BugEngine::RTTI::ClassIndex_u32 << 16)))
be_pod uint4x4
{
    const uint4& operator[](u32) const;
    uint4& operator[](u32);
};

#endif

/**************************************************************************************************/
#endif
