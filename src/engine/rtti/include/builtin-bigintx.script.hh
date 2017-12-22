/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_BUILTIN_BIGINTX_SCRIPT_HH_
#define BE_RTTI_BUILTIN_BIGINTX_SCRIPT_HH_
/**************************************************************************************************/
#include    <rtti/stdafx.h>
#include    <rtti/classinfo.script.hh>
#include    <rtti/builtin.hh>


#if 0
namespace BugEngine
{

be_tag(Index(BugEngine::RTTI::ClassType_Vector2
          + (BugEngine::RTTI::ClassIndex_i64 << 16)))
be_pod bigint2
{
    i64 operator[](u32) const;
    i64& operator[](u32);
};


be_tag(Index(BugEngine::RTTI::ClassType_Vector3
          + (BugEngine::RTTI::ClassIndex_i64 << 16)))
be_pod bigint3
{
    i64 operator[](u32) const;
    i64& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector4
          + (BugEngine::RTTI::ClassIndex_i64 << 16)))
be_pod bigint4
{
    i64 operator[](u32) const;
    i64& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector8
          + (BugEngine::RTTI::ClassIndex_i64 << 16)))
be_pod bigint8
{
    i64 operator[](u32) const;
    i64& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector16
          + (BugEngine::RTTI::ClassIndex_i64 << 16)))
be_pod bigint16
{
    i64 operator[](u32) const;
    i64& operator[](u32);
};

}
#endif

/**************************************************************************************************/
#endif

