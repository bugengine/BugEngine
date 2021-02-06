/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_BUILTIN_BIGUINTX_SCRIPT_HH_
#define BE_RTTI_BUILTIN_BIGUINTX_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/rtti/stdafx.h>
#include <bugengine/rtti/builtin.hh>
#include <bugengine/rtti/classinfo.script.hh>

#if 0
namespace BugEngine
{

be_tag(Index(BugEngine::RTTI::ClassType_Vector2
          + (BugEngine::RTTI::ClassIndex_u64 << 16)))
be_pod biguint2
{
    u64 operator[](u32) const;
    u64& operator[](u32);
};


be_tag(Index(BugEngine::RTTI::ClassType_Vector3
          + (BugEngine::RTTI::ClassIndex_u64 << 16)))
be_pod biguint3
{
    u64 operator[](u32) const;
    u64& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector4
          + (BugEngine::RTTI::ClassIndex_u64 << 16)))
be_pod biguint4
{
    u64 operator[](u32) const;
    u64& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector8
          + (BugEngine::RTTI::ClassIndex_u64 << 16)))
be_pod biguint8
{
    u64 operator[](u32) const;
    u64& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector16
          + (BugEngine::RTTI::ClassIndex_u64 << 16)))
be_pod biguint16
{
    u64 operator[](u32) const;
    u64& operator[](u32);
};

}
#endif

/**************************************************************************************************/
#endif
