/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_BUILTIN_SCRIPT_HH_
#define BE_RTTI_BUILTIN_SCRIPT_HH_
/**************************************************************************************************/
#include    <rtti/stdafx.h>
#include    <rtti/classinfo.script.hh>
#include    <rtti/builtin.hh>


#if 0

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

be_tag(Index(BugEngine::RTTI::ClassType_Vector2
          + (BugEngine::RTTI::ClassIndex_u16 << 16)))
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

be_tag(Index(BugEngine::RTTI::ClassType_Vector3
          + (BugEngine::RTTI::ClassIndex_u16 << 16)))
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

be_tag(Index(BugEngine::RTTI::ClassType_Vector4
          + (BugEngine::RTTI::ClassIndex_u16 << 16)))
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
be_tag(Index(BugEngine::RTTI::ClassType_Matrix
          + (BugEngine::RTTI::ClassIndex_u16 << 16)))
be_pod ushort4x4
{
    const ushort4& operator[](u32) const;
    ushort4& operator[](u32);
};

#endif

/**************************************************************************************************/
#endif
