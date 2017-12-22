/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_BUILTIN_USHORTX_SCRIPT_HH_
#define BE_RTTI_BUILTIN_USHORTX_SCRIPT_HH_
/**************************************************************************************************/
#include    <rtti/stdafx.h>
#include    <rtti/classinfo.script.hh>
#include    <rtti/builtin.hh>


#if 0

be_tag(Index(BugEngine::RTTI::ClassType_Vector2
          + (BugEngine::RTTI::ClassIndex_u16 << 16)))
be_pod ushort2
{
    u16 operator[](u32) const;
    u16& operator[](u32);
};


be_tag(Index(BugEngine::RTTI::ClassType_Vector3
          + (BugEngine::RTTI::ClassIndex_u16 << 16)))
be_pod ushort3
{
    u16 operator[](u32) const;
    u16& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector4
          + (BugEngine::RTTI::ClassIndex_u16 << 16)))
be_pod ushort4
{
    u16 operator[](u32) const;
    u16& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector8
          + (BugEngine::RTTI::ClassIndex_u16 << 16)))
be_pod ushort8
{
    u16 operator[](u32) const;
    u16& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector16
          + (BugEngine::RTTI::ClassIndex_u16 << 16)))
be_pod ushort16
{
    u16 operator[](u32) const;
    u16& operator[](u32);
};

#endif

/**************************************************************************************************/
#endif

