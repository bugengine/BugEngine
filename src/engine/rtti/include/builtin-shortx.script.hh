/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_BUILTIN_SHORTX_SCRIPT_HH_
#define BE_RTTI_BUILTIN_SHORTX_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/rtti/stdafx.h>
#include <bugengine/rtti/builtin.hh>
#include <bugengine/rtti/classinfo.script.hh>

#if 0
namespace BugEngine
{

be_tag(Index(BugEngine::RTTI::ClassType_Vector2
          + (BugEngine::RTTI::ClassIndex_i16 << 16)))
be_pod short2
{
    i16 operator[](u32) const;
    i16& operator[](u32);
};


be_tag(Index(BugEngine::RTTI::ClassType_Vector3
          + (BugEngine::RTTI::ClassIndex_i16 << 16)))
be_pod short3
{
    i16 operator[](u32) const;
    i16& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector4
          + (BugEngine::RTTI::ClassIndex_i16 << 16)))
be_pod short4
{
    i16 operator[](u32) const;
    i16& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector8
          + (BugEngine::RTTI::ClassIndex_i16 << 16)))
be_pod short8
{
    i16 operator[](u32) const;
    i16& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector16
          + (BugEngine::RTTI::ClassIndex_i16 << 16)))
be_pod short16
{
    i16 operator[](u32) const;
    i16& operator[](u32);
};

}
#endif

/**************************************************************************************************/
#endif
