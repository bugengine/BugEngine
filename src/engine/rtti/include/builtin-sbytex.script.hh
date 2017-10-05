/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_BUILTIN_SCRIPT_HH_
#define BE_RTTI_BUILTIN_SCRIPT_HH_
/**************************************************************************************************/
#include    <rtti/stdafx.h>
#include    <rtti/classinfo.script.hh>
#include    <rtti/builtin.hh>


#if 0

be_pod sbyte1x2
{
    i8 operator[](u32) const;
    i8& operator[](u32);
};
be_pod sbyte1x3
{
    i8 operator[](u32) const;
    i8& operator[](u32);
};
be_pod sbyte1x4
{
    i8 operator[](u32) const;
    i8& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector2
          + (BugEngine::RTTI::ClassIndex_i8 << 16)))
be_pod sbyte2
{
    i8 operator[](u32) const;
    i8& operator[](u32);
};
be_pod sbyte2x2
{
    const sbyte2& operator[](u32) const;
    sbyte2& operator[](u32);
};
be_pod sbyte2x3
{
    const sbyte2& operator[](u32) const;
    sbyte2& operator[](u32);
};
be_pod sbyte2x4
{
    const sbyte2& operator[](u32) const;
    sbyte2& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector3
          + (BugEngine::RTTI::ClassIndex_i8 << 16)))
be_pod sbyte3
{
    i8 operator[](u32) const;
    i8& operator[](u32);
};
be_pod sbyte3x2
{
    const sbyte3& operator[](u32) const;
    sbyte3& operator[](u32);
};
be_pod sbyte3x3
{
    const sbyte3& operator[](u32) const;
    sbyte3& operator[](u32);
};
be_pod sbyte3x4
{
    const sbyte3& operator[](u32) const;
    sbyte3& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector4
          + (BugEngine::RTTI::ClassIndex_i8 << 16)))
be_pod sbyte4
{
    i8 operator[](u32) const;
    i8& operator[](u32);
};
be_pod sbyte4x2
{
    const sbyte4& operator[](u32) const;
    sbyte4& operator[](u32);
};
be_pod sbyte4x3
{
    const sbyte4& operator[](u32) const;
    sbyte4& operator[](u32);
};
be_tag(Index(BugEngine::RTTI::ClassType_Matrix
          + (BugEngine::RTTI::ClassIndex_i8 << 16)))
be_pod sbyte4x4
{
    const sbyte4& operator[](u32) const;
    sbyte4& operator[](u32);
};

#endif

/**************************************************************************************************/
#endif
