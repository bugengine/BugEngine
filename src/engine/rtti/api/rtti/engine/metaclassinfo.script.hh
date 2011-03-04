/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_METACLASSINFO_HH_
#define BE_RTTI_ENGINE_METACLASSINFO_HH_
/*****************************************************************************/
#include    <rtti/classinfo.script.hh>
#include    <rtti/namespace.script.hh>

namespace BugEngine
{

namespace RTTI
{

class be_api(RTTI) Namespace::MetaClassInfo : public ClassInfo
{
public:
    MetaClassInfo(const inamespace& name, ref<const ClassInfo> parent);
    MetaClassInfo(const inamespace& name, ref<const ClassInfo> parent, u32 size, i32 offset);
    ~MetaClassInfo();

    virtual Value get(Value& from, istring name) const override;
};

class be_api(RTTI) ClassInfo::MetaClassInfo : public RTTI::Namespace::MetaClassInfo
{
public:
    MetaClassInfo(const inamespace& name, ref<const ClassInfo> parent);
    MetaClassInfo(const inamespace& name, ref<const ClassInfo> parent, u32 size, i32 offset);
    ~MetaClassInfo();

    virtual Value get(Value& from, istring name) const override;
};

}}

/*****************************************************************************/
#endif

