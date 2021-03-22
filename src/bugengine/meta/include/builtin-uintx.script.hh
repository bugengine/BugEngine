/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_META_BUILTIN_UINTX_SCRIPT_HH_
#define BE_META_BUILTIN_UINTX_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/meta/stdafx.h>
#include <bugengine/meta/builtin.hh>
#include <bugengine/meta/classinfo.script.hh>

#if 0
namespace BugEngine
{

be_tag(Index(BugEngine::Meta::ClassType_Vector2
          + (BugEngine::Meta::ClassIndex_u32 << 16)))
be_pod uint2
{
    u32 operator[](u32) const;
    u32& operator[](u32);
};


be_tag(Index(BugEngine::Meta::ClassType_Vector3
          + (BugEngine::Meta::ClassIndex_u32 << 16)))
be_pod uint3
{
    u32 operator[](u32) const;
    u32& operator[](u32);
};

be_tag(Index(BugEngine::Meta::ClassType_Vector4
          + (BugEngine::Meta::ClassIndex_u32 << 16)))
be_pod uint4
{
    u32 operator[](u32) const;
    u32& operator[](u32);
};

be_tag(Index(BugEngine::Meta::ClassType_Vector8
          + (BugEngine::Meta::ClassIndex_u32 << 16)))
be_pod uint8
{
    u32 operator[](u32) const;
    u32& operator[](u32);
};

be_tag(Index(BugEngine::Meta::ClassType_Vector16
          + (BugEngine::Meta::ClassIndex_u32 << 16)))
be_pod uint16
{
    u32 operator[](u32) const;
    u32& operator[](u32);
};

}
#endif

/**************************************************************************************************/
#endif
