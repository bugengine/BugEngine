/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_BUILTIN_SCRIPT_HH_
#define BE_RTTI_BUILTIN_SCRIPT_HH_
/**************************************************************************************************/
#include    <rtti/stdafx.h>
#include    <rtti/classinfo.script.hh>
#include    <rtti/builtin.hh>


#if 0

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

be_tag(Index(BugEngine::RTTI::ClassType_Vector2
          + (BugEngine::RTTI::ClassIndex_u64 << 16)))
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

be_tag(Index(BugEngine::RTTI::ClassType_Vector3
          + (BugEngine::RTTI::ClassIndex_u64 << 16)))
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

be_tag(Index(BugEngine::RTTI::ClassType_Vector4
          + (BugEngine::RTTI::ClassIndex_u64 << 16)))
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
be_tag(Index(BugEngine::RTTI::ClassType_Matrix
          + (BugEngine::RTTI::ClassIndex_u64 << 16)))
be_pod biguint4x4
{
    const biguint4& operator[](u32) const;
    biguint4& operator[](u32);
};

#endif

/**************************************************************************************************/
#endif
