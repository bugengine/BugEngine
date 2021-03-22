/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_META_BUILTIN_SHORTX_SCRIPT_HH_
#define BE_META_BUILTIN_SHORTX_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/meta/stdafx.h>
#include <bugengine/meta/builtin.hh>
#include <bugengine/meta/classinfo.script.hh>

#if 0
namespace BugEngine
{

be_tag(Index(BugEngine::Meta::ClassType_Vector2
          + (BugEngine::Meta::ClassIndex_i16 << 16)))
be_pod short2
{
    i16 operator[](u32) const;
    i16& operator[](u32);
};


be_tag(Index(BugEngine::Meta::ClassType_Vector3
          + (BugEngine::Meta::ClassIndex_i16 << 16)))
be_pod short3
{
    i16 operator[](u32) const;
    i16& operator[](u32);
};

be_tag(Index(BugEngine::Meta::ClassType_Vector4
          + (BugEngine::Meta::ClassIndex_i16 << 16)))
be_pod short4
{
    i16 operator[](u32) const;
    i16& operator[](u32);
};

be_tag(Index(BugEngine::Meta::ClassType_Vector8
          + (BugEngine::Meta::ClassIndex_i16 << 16)))
be_pod short8
{
    i16 operator[](u32) const;
    i16& operator[](u32);
};

be_tag(Index(BugEngine::Meta::ClassType_Vector16
          + (BugEngine::Meta::ClassIndex_i16 << 16)))
be_pod short16
{
    i16 operator[](u32) const;
    i16& operator[](u32);
};

}
#endif

/**************************************************************************************************/
#endif
