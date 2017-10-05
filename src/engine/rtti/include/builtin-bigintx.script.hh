/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_BUILTIN_SCRIPT_HH_
#define BE_RTTI_BUILTIN_SCRIPT_HH_
/**************************************************************************************************/
#include    <rtti/stdafx.h>
#include    <rtti/classinfo.script.hh>
#include    <rtti/builtin.hh>


#if 0

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

be_tag(Index(BugEngine::RTTI::ClassType_Vector2
          + (BugEngine::RTTI::ClassIndex_i64 << 16)))
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

be_tag(Index(BugEngine::RTTI::ClassType_Vector3
          + (BugEngine::RTTI::ClassIndex_i64 << 16)))
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

be_tag(Index(BugEngine::RTTI::ClassType_Vector4
          + (BugEngine::RTTI::ClassIndex_i64 << 16)))
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
be_tag(Index(BugEngine::RTTI::ClassType_Matrix
          + (BugEngine::RTTI::ClassIndex_i64 << 16)))
be_pod bigint4x4
{
    const bigint4& operator[](u32) const;
    bigint4& operator[](u32);
};

#endif

/**************************************************************************************************/
#endif
