/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_META_BUILTIN_DOUBLEX_SCRIPT_HH_
#define BE_META_BUILTIN_DOUBLEX_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/meta/stdafx.h>
#include <bugengine/meta/builtin.hh>
#include <bugengine/meta/classinfo.script.hh>

#if 0
namespace BugEngine
{

be_tag(Index(BugEngine::Meta::ClassType_Vector2
          + (BugEngine::Meta::ClassIndex_double << 16)))
be_pod double2
{
    double operator[](u32) const;
    double& operator[](u32);
};


be_tag(Index(BugEngine::Meta::ClassType_Vector3
          + (BugEngine::Meta::ClassIndex_double << 16)))
be_pod double3
{
    double operator[](u32) const;
    double& operator[](u32);
};

be_tag(Index(BugEngine::Meta::ClassType_Vector4
          + (BugEngine::Meta::ClassIndex_double << 16)))
be_pod double4
{
    double operator[](u32) const;
    double& operator[](u32);
};

be_tag(Index(BugEngine::Meta::ClassType_Vector8
          + (BugEngine::Meta::ClassIndex_double << 16)))
be_pod double8
{
    double operator[](u32) const;
    double& operator[](u32);
};

be_tag(Index(BugEngine::Meta::ClassType_Vector16
          + (BugEngine::Meta::ClassIndex_double << 16)))
be_pod double16
{
    double operator[](u32) const;
    double& operator[](u32);
};

}
#endif

/**************************************************************************************************/
#endif
