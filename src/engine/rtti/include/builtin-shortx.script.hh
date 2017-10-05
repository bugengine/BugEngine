/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_BUILTIN_SCRIPT_HH_
#define BE_RTTI_BUILTIN_SCRIPT_HH_
/**************************************************************************************************/
#include    <rtti/stdafx.h>
#include    <rtti/classinfo.script.hh>
#include    <rtti/builtin.hh>


#if 0

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

be_tag(Index(BugEngine::RTTI::ClassType_Vector2
          + (BugEngine::RTTI::ClassIndex_i16 << 16)))
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

be_tag(Index(BugEngine::RTTI::ClassType_Vector3
          + (BugEngine::RTTI::ClassIndex_i16 << 16)))
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

be_tag(Index(BugEngine::RTTI::ClassType_Vector4
          + (BugEngine::RTTI::ClassIndex_i16 << 16)))
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
be_tag(Index(BugEngine::RTTI::ClassType_Matrix
          + (BugEngine::RTTI::ClassIndex_i16 << 16)))
be_pod short4x4
{
    const short4& operator[](u32) const;
    short4& operator[](u32);
};

#endif

/**************************************************************************************************/
#endif
