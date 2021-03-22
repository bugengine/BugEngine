/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_META_BUILTIN_FLOATX_SCRIPT_HH_
#define BE_META_BUILTIN_FLOATX_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/meta/stdafx.h>
#include <bugengine/meta/builtin.hh>
#include <bugengine/meta/classinfo.script.hh>

#if 0
namespace BugEngine
{

be_tag(Index(BugEngine::Meta::ClassType_Vector2
          + (BugEngine::Meta::ClassIndex_float << 16)))
be_pod float2
{
    float operator[](u32) const;
    float& operator[](u32);
};


be_tag(Index(BugEngine::Meta::ClassType_Vector3
          + (BugEngine::Meta::ClassIndex_float << 16)))
be_pod float3
{
    float operator[](u32) const;
    float& operator[](u32);
};

be_tag(Index(BugEngine::Meta::ClassType_Vector4
          + (BugEngine::Meta::ClassIndex_float << 16)))
be_pod float4
{
    float operator[](u32) const;
    float& operator[](u32);
};

be_tag(Index(BugEngine::Meta::ClassType_Vector8
          + (BugEngine::Meta::ClassIndex_float << 16)))
be_pod float8
{
    float operator[](u32) const;
    float& operator[](u32);
};

be_tag(Index(BugEngine::Meta::ClassType_Vector16
          + (BugEngine::Meta::ClassIndex_float << 16)))
be_pod float16
{
    float operator[](u32) const;
    float& operator[](u32);
};

}
#endif

/**************************************************************************************************/
#endif
