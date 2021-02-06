/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_BUILTIN_INTX_SCRIPT_HH_
#define BE_RTTI_BUILTIN_INTX_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/rtti/stdafx.h>
#include <bugengine/rtti/builtin.hh>
#include <bugengine/rtti/classinfo.script.hh>

#if 0
namespace BugEngine
{

be_tag(Index(BugEngine::RTTI::ClassType_Vector2
          + (BugEngine::RTTI::ClassIndex_i32 << 16)))
be_pod int2
{
    i32 operator[](u32) const;
    i32& operator[](u32);
};


be_tag(Index(BugEngine::RTTI::ClassType_Vector3
          + (BugEngine::RTTI::ClassIndex_i32 << 16)))
be_pod int3
{
    i32 operator[](u32) const;
    i32& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector4
          + (BugEngine::RTTI::ClassIndex_i32 << 16)))
be_pod int4
{
    i32 operator[](u32) const;
    i32& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector8
          + (BugEngine::RTTI::ClassIndex_i32 << 16)))
be_pod int8
{
    i32 operator[](u32) const;
    i32& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector16
          + (BugEngine::RTTI::ClassIndex_i32 << 16)))
be_pod int16
{
    i32 operator[](u32) const;
    i32& operator[](u32);
};

}
#endif

/**************************************************************************************************/
#endif
