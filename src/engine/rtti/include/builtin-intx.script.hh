/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_BUILTIN_SCRIPT_HH_
#define BE_RTTI_BUILTIN_SCRIPT_HH_
/**************************************************************************************************/
#include    <rtti/stdafx.h>
#include    <rtti/classinfo.script.hh>
#include    <rtti/builtin.hh>


#if 0

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

be_tag(Index(BugEngine::RTTI::ClassType_Vector2
          + (BugEngine::RTTI::ClassIndex_i32 << 16)))
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

be_tag(Index(BugEngine::RTTI::ClassType_Vector3
          + (BugEngine::RTTI::ClassIndex_i32 << 16)))
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

be_tag(Index(BugEngine::RTTI::ClassType_Vector4
          + (BugEngine::RTTI::ClassIndex_i32 << 16)))
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
be_tag(Index(BugEngine::RTTI::ClassType_Matrix
          + (BugEngine::RTTI::ClassIndex_i32 << 16)))
be_pod int4x4
{
    const int4& operator[](u32) const;
    int4& operator[](u32);
};

#endif

/**************************************************************************************************/
#endif
