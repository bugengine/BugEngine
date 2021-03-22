/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_META_BUILTIN_UBYTEX_SCRIPT_HH_
#define BE_META_BUILTIN_UBYTEX_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/meta/stdafx.h>
#include <bugengine/meta/builtin.hh>
#include <bugengine/meta/classinfo.script.hh>

#if 0
namespace BugEngine
{

be_tag(Index(BugEngine::Meta::ClassType_Vector2
          + (BugEngine::Meta::ClassIndex_u8 << 16)))
be_pod ubyte2
{
    u8 operator[](u32) const;
    u8& operator[](u32);
};


be_tag(Index(BugEngine::Meta::ClassType_Vector3
          + (BugEngine::Meta::ClassIndex_u8 << 16)))
be_pod ubyte3
{
    u8 operator[](u32) const;
    u8& operator[](u32);
};

be_tag(Index(BugEngine::Meta::ClassType_Vector4
          + (BugEngine::Meta::ClassIndex_u8 << 16)))
be_pod ubyte4
{
    u8 operator[](u32) const;
    u8& operator[](u32);
};

be_tag(Index(BugEngine::Meta::ClassType_Vector8
          + (BugEngine::Meta::ClassIndex_u8 << 16)))
be_pod ubyte8
{
    u8 operator[](u32) const;
    u8& operator[](u32);
};

be_tag(Index(BugEngine::Meta::ClassType_Vector16
          + (BugEngine::Meta::ClassIndex_u8 << 16)))
be_pod ubyte16
{
    u8 operator[](u32) const;
    u8& operator[](u32);
};

}
#endif

/**************************************************************************************************/
#endif
