/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_META_BUILTIN_INTX_SCRIPT_HH_
#define BE_META_BUILTIN_INTX_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/meta/stdafx.h>
#include <bugengine/meta/builtin.hh>
#include <bugengine/meta/classinfo.script.hh>

#if 0
namespace BugEngine
{

be_tag(Index(BugEngine::Meta::ClassType_Vector2
          + (BugEngine::Meta::ClassIndex_i32 << 16)))
be_pod int2
{
    i32 operator[](u32) const;
    i32& operator[](u32);
};


be_tag(Index(BugEngine::Meta::ClassType_Vector3
          + (BugEngine::Meta::ClassIndex_i32 << 16)))
be_pod int3
{
    i32 operator[](u32) const;
    i32& operator[](u32);
};

be_tag(Index(BugEngine::Meta::ClassType_Vector4
          + (BugEngine::Meta::ClassIndex_i32 << 16)))
be_pod int4
{
    i32 operator[](u32) const;
    i32& operator[](u32);
};

be_tag(Index(BugEngine::Meta::ClassType_Vector8
          + (BugEngine::Meta::ClassIndex_i32 << 16)))
be_pod int8
{
    i32 operator[](u32) const;
    i32& operator[](u32);
};

be_tag(Index(BugEngine::Meta::ClassType_Vector16
          + (BugEngine::Meta::ClassIndex_i32 << 16)))
be_pod int16
{
    i32 operator[](u32) const;
    i32& operator[](u32);
};

}
#endif

/**************************************************************************************************/
#endif
