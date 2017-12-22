/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_BUILTIN_BYTEX_SCRIPT_HH_
#define BE_RTTI_BUILTIN_BYTEX_SCRIPT_HH_
/**************************************************************************************************/
#include    <rtti/stdafx.h>
#include    <rtti/classinfo.script.hh>
#include    <rtti/builtin.hh>


#if 0

be_tag(Index(BugEngine::RTTI::ClassType_Vector2
          + (BugEngine::RTTI::ClassIndex_u8 << 16)))
be_pod byte2
{
    u8 operator[](u32) const;
    u8& operator[](u32);
};


be_tag(Index(BugEngine::RTTI::ClassType_Vector3
          + (BugEngine::RTTI::ClassIndex_u8 << 16)))
be_pod byte3
{
    u8 operator[](u32) const;
    u8& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector4
          + (BugEngine::RTTI::ClassIndex_u8 << 16)))
be_pod byte4
{
    u8 operator[](u32) const;
    u8& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector8
          + (BugEngine::RTTI::ClassIndex_u8 << 16)))
be_pod byte8
{
    u8 operator[](u32) const;
    u8& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector16
          + (BugEngine::RTTI::ClassIndex_u8 << 16)))
be_pod byte16
{
    u8 operator[](u32) const;
    u8& operator[](u32);
};

#endif

/**************************************************************************************************/
#endif

