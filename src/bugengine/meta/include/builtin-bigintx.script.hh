/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_META_BUILTIN_BIGINTX_SCRIPT_HH_
#define BE_META_BUILTIN_BIGINTX_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/meta/stdafx.h>
#include <bugengine/meta/builtin.hh>
#include <bugengine/meta/classinfo.script.hh>

#if 0
namespace BugEngine
{

be_tag(Index(BugEngine::Meta::ClassType_Vector2
          + (BugEngine::Meta::ClassIndex_i64 << 16)))
be_pod bigint2
{
    i64 operator[](u32) const;
    i64& operator[](u32);
};


be_tag(Index(BugEngine::Meta::ClassType_Vector3
          + (BugEngine::Meta::ClassIndex_i64 << 16)))
be_pod bigint3
{
    i64 operator[](u32) const;
    i64& operator[](u32);
};

be_tag(Index(BugEngine::Meta::ClassType_Vector4
          + (BugEngine::Meta::ClassIndex_i64 << 16)))
be_pod bigint4
{
    i64 operator[](u32) const;
    i64& operator[](u32);
};

be_tag(Index(BugEngine::Meta::ClassType_Vector8
          + (BugEngine::Meta::ClassIndex_i64 << 16)))
be_pod bigint8
{
    i64 operator[](u32) const;
    i64& operator[](u32);
};

be_tag(Index(BugEngine::Meta::ClassType_Vector16
          + (BugEngine::Meta::ClassIndex_i64 << 16)))
be_pod bigint16
{
    i64 operator[](u32) const;
    i64& operator[](u32);
};

}
#endif

/**************************************************************************************************/
#endif
