/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/package.hh>
#include    <rtti/namespace.hh>


namespace BugEngine { namespace RTTI
{

be_abstractmetaclass_impl("",Package);

Package::Package()
:   m_refcount(0)
,   m_name("")
{
}

Package::Package(const istring& name, weak<Package> parent)
:   m_refcount(0)
,   m_name(name)
,   m_parent(parent)
{
}

Package::~Package()
{
    be_assert(m_refcount == 0, "destroying package %s that is still being used" | m_name.c_str());
}

ref<Package> Package::MetaClass::create(const ipath& name) const
{
    UNUSED(name);
    throw 0;
}

void Package::load()
{
    if(!m_refcount)
        doload();
    m_refcount++;
}

void Package::unload()
{
    if(!--m_refcount)
        dounload(); 
}

weak<Package> Package::get(const inamespace& name)
{
    UNUSED(name);
/*  static Package s_root;
    Package* result = &s_root;
    for(size_t i = 0; i < name.size(); ++i)
    {
        stdhash::hash_map<istring,ref<Package> >::iterator it = result->m_children.find(name[i]);
        if(it == result->m_children.end())
            return 0;
        result = *it->second;
    }
    return result;*/
    return weak<Package>();
}

}}
