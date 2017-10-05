/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_BUILTIN_SCRIPT_HH_
#define BE_RTTI_BUILTIN_SCRIPT_HH_
/**************************************************************************************************/
#include    <rtti/stdafx.h>
#include    <rtti/classinfo.script.hh>
#include    <rtti/builtin.hh>


#if 0

be_pod byte1x2
{
    u8 operator[](u32) const;
    u8& operator[](u32);
};
be_pod byte1x3
{
    u8 operator[](u32) const;
    u8& operator[](u32);
};
be_pod byte1x4
{
    u8 operator[](u32) const;
    u8& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector2
          + (BugEngine::RTTI::ClassIndex_u8 << 16)))
be_pod byte2
{
    u8 operator[](u32) const;
    u8& operator[](u32);
};
be_pod byte2x2
{
    const byte2& operator[](u32) const;
    byte2& operator[](u32);
};
be_pod byte2x3
{
    const byte2& operator[](u32) const;
    byte2& operator[](u32);
};
be_pod byte2x4
{
    const byte2& operator[](u32) const;
    byte2& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector3
          + (BugEngine::RTTI::ClassIndex_u8 << 16)))
be_pod byte3
{
    u8 operator[](u32) const;
    u8& operator[](u32);
};
be_pod byte3x2
{
    const byte3& operator[](u32) const;
    byte3& operator[](u32);
};
be_pod byte3x3
{
    const byte3& operator[](u32) const;
    byte3& operator[](u32);
};
be_pod byte3x4
{
    const byte3& operator[](u32) const;
    byte3& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector4
          + (BugEngine::RTTI::ClassIndex_u8 << 16)))
be_pod byte4
{
    u8 operator[](u32) const;
    u8& operator[](u32);
};
be_pod byte4x2
{
    const byte4& operator[](u32) const;
    byte4& operator[](u32);
};
be_pod byte4x3
{
    const byte4& operator[](u32) const;
    byte4& operator[](u32);
};
be_tag(Index(BugEngine::RTTI::ClassType_Matrix
          + (BugEngine::RTTI::ClassIndex_u8 << 16)))
be_pod byte4x4
{
    const byte4& operator[](u32) const;
    byte4& operator[](u32);
};

#endif

/**************************************************************************************************/
#endif
