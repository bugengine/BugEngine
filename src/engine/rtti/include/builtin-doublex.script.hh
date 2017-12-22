/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_BUILTIN_DOUBLEX_SCRIPT_HH_
#define BE_RTTI_BUILTIN_DOUBLEX_SCRIPT_HH_
/**************************************************************************************************/
#include    <rtti/stdafx.h>
#include    <rtti/classinfo.script.hh>
#include    <rtti/builtin.hh>


#if 0
namespace BugEngine
{

be_tag(Index(BugEngine::RTTI::ClassType_Vector2
          + (BugEngine::RTTI::ClassIndex_double << 16)))
be_pod double2
{
    double operator[](u32) const;
    double& operator[](u32);
};


be_tag(Index(BugEngine::RTTI::ClassType_Vector3
          + (BugEngine::RTTI::ClassIndex_double << 16)))
be_pod double3
{
    double operator[](u32) const;
    double& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector4
          + (BugEngine::RTTI::ClassIndex_double << 16)))
be_pod double4
{
    double operator[](u32) const;
    double& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector8
          + (BugEngine::RTTI::ClassIndex_double << 16)))
be_pod double8
{
    double operator[](u32) const;
    double& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector16
          + (BugEngine::RTTI::ClassIndex_double << 16)))
be_pod double16
{
    double operator[](u32) const;
    double& operator[](u32);
};

}
#endif

/**************************************************************************************************/
#endif

