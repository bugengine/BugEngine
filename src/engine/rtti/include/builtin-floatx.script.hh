/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_BUILTIN_FLOATX_SCRIPT_HH_
#define BE_RTTI_BUILTIN_FLOATX_SCRIPT_HH_
/**************************************************************************************************/
#include    <rtti/stdafx.h>
#include    <rtti/classinfo.script.hh>
#include    <rtti/builtin.hh>


#if 0
namespace BugEngine
{

be_tag(Index(BugEngine::RTTI::ClassType_Vector2
          + (BugEngine::RTTI::ClassIndex_float << 16)))
be_pod float2
{
    float operator[](u32) const;
    float& operator[](u32);
};


be_tag(Index(BugEngine::RTTI::ClassType_Vector3
          + (BugEngine::RTTI::ClassIndex_float << 16)))
be_pod float3
{
    float operator[](u32) const;
    float& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector4
          + (BugEngine::RTTI::ClassIndex_float << 16)))
be_pod float4
{
    float operator[](u32) const;
    float& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector8
          + (BugEngine::RTTI::ClassIndex_float << 16)))
be_pod float8
{
    float operator[](u32) const;
    float& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector16
          + (BugEngine::RTTI::ClassIndex_float << 16)))
be_pod float16
{
    float operator[](u32) const;
    float& operator[](u32);
};

}
#endif

/**************************************************************************************************/
#endif

