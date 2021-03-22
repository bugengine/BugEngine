/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_META_BUILTIN_BIGUINTX_SCRIPT_HH_
#define BE_META_BUILTIN_BIGUINTX_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/meta/stdafx.h>
#include <bugengine/meta/builtin.hh>
#include <bugengine/meta/classinfo.script.hh>

#if 0
namespace BugEngine
{

be_tag(Index(BugEngine::Meta::ClassType_Vector2
          + (BugEngine::Meta::ClassIndex_u64 << 16)))
be_pod biguint2
{
    u64 operator[](u32) const;
    u64& operator[](u32);
};


be_tag(Index(BugEngine::Meta::ClassType_Vector3
          + (BugEngine::Meta::ClassIndex_u64 << 16)))
be_pod biguint3
{
    u64 operator[](u32) const;
    u64& operator[](u32);
};

be_tag(Index(BugEngine::Meta::ClassType_Vector4
          + (BugEngine::Meta::ClassIndex_u64 << 16)))
be_pod biguint4
{
    u64 operator[](u32) const;
    u64& operator[](u32);
};

be_tag(Index(BugEngine::Meta::ClassType_Vector8
          + (BugEngine::Meta::ClassIndex_u64 << 16)))
be_pod biguint8
{
    u64 operator[](u32) const;
    u64& operator[](u32);
};

be_tag(Index(BugEngine::Meta::ClassType_Vector16
          + (BugEngine::Meta::ClassIndex_u64 << 16)))
be_pod biguint16
{
    u64 operator[](u32) const;
    u64& operator[](u32);
};

}
#endif

/**************************************************************************************************/
#endif
