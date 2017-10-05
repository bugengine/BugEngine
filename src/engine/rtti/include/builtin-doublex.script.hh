/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_BUILTIN_SCRIPT_HH_
#define BE_RTTI_BUILTIN_SCRIPT_HH_
/**************************************************************************************************/
#include    <rtti/stdafx.h>
#include    <rtti/classinfo.script.hh>
#include    <rtti/builtin.hh>


#if 0

be_pod double1x2
{
    double operator[](u32) const;
    double& operator[](u32);
};
be_pod double1x3
{
    double operator[](u32) const;
    double& operator[](u32);
};
be_pod double1x4
{
    double operator[](u32) const;
    double& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector2
          + (BugEngine::RTTI::ClassIndex_double << 16)))
be_pod double2
{
    double operator[](u32) const;
    double& operator[](u32);
};
be_pod double2x2
{
    const double2& operator[](u32) const;
    double2& operator[](u32);
};
be_pod double2x3
{
    const double2& operator[](u32) const;
    double2& operator[](u32);
};
be_pod double2x4
{
    const double2& operator[](u32) const;
    double2& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector3
          + (BugEngine::RTTI::ClassIndex_double << 16)))
be_pod double3
{
    double operator[](u32) const;
    double& operator[](u32);
};
be_pod double3x2
{
    const double3& operator[](u32) const;
    double3& operator[](u32);
};
be_pod double3x3
{
    const double3& operator[](u32) const;
    double3& operator[](u32);
};
be_pod double3x4
{
    const double3& operator[](u32) const;
    double3& operator[](u32);
};

be_tag(Index(BugEngine::RTTI::ClassType_Vector4
          + (BugEngine::RTTI::ClassIndex_double << 16)))
be_pod double4
{
    double operator[](u32) const;
    double& operator[](u32);
};
be_pod double4x2
{
    const double4& operator[](u32) const;
    double4& operator[](u32);
};
be_pod double4x3
{
    const double4& operator[](u32) const;
    double4& operator[](u32);
};
be_tag(Index(BugEngine::RTTI::ClassType_Matrix
          + (BugEngine::RTTI::ClassIndex_double << 16)))
be_pod double4x4
{
    const double4& operator[](u32) const;
    double4& operator[](u32);
};

#endif

/**************************************************************************************************/
#endif
