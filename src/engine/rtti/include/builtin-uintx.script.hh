/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_BUILTIN_UINTX_SCRIPT_HH_
#define BE_RTTI_BUILTIN_UINTX_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/rtti/stdafx.h>
#include <bugengine/rtti/builtin.hh>
#include <bugengine/rtti/classinfo.script.hh>

#if 0
namespace BugEngine
{

be_tag(Index(BugEngine::RTTI::ClassType_Vector2
          + (BugEngine::RTTI::ClassIndex_u32 << 16)))
be_pod uint2
{
    u32 operator[](u32) const;
    u32& operator[](u32);
};


be_tag(Index(BugEngine::RTTI::ClassType_Vector3
          + (BugEngine::RTTI::ClassIndex_u32 << 16)))
be_pod uint3
{
    u32 operator[](u32) const;
    u32& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector4
          + (BugEngine::RTTI::ClassIndex_u32 << 16)))
be_pod uint4
{
    u32 operator[](u32) const;
    u32& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector8
          + (BugEngine::RTTI::ClassIndex_u32 << 16)))
be_pod uint8
{
    u32 operator[](u32) const;
    u32& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector16
          + (BugEngine::RTTI::ClassIndex_u32 << 16)))
be_pod uint16
{
    u32 operator[](u32) const;
    u32& operator[](u32);
};

}
#endif

/**************************************************************************************************/
#endif
