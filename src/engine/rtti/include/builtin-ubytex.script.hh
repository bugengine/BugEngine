/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_BUILTIN_SBYTEX_SCRIPT_HH_
#define BE_RTTI_BUILTIN_SBYTEX_SCRIPT_HH_
/**************************************************************************************************/
#include    <rtti/stdafx.h>
#include    <rtti/classinfo.script.hh>
#include    <rtti/builtin.hh>


#if 0

be_tag(Index(BugEngine::RTTI::ClassType_Vector2
          + (BugEngine::RTTI::ClassIndex_i8 << 16)))
be_pod sbyte2
{
    i8 operator[](u32) const;
    i8& operator[](u32);
};


be_tag(Index(BugEngine::RTTI::ClassType_Vector3
          + (BugEngine::RTTI::ClassIndex_i8 << 16)))
be_pod sbyte3
{
    i8 operator[](u32) const;
    i8& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector4
          + (BugEngine::RTTI::ClassIndex_i8 << 16)))
be_pod sbyte4
{
    i8 operator[](u32) const;
    i8& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector8
          + (BugEngine::RTTI::ClassIndex_i8 << 16)))
be_pod sbyte8
{
    i8 operator[](u32) const;
    i8& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector16
          + (BugEngine::RTTI::ClassIndex_i8 << 16)))
be_pod sbyte16
{
    i8 operator[](u32) const;
    i8& operator[](u32);
};

#endif

/**************************************************************************************************/
#endif

