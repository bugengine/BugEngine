/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

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

Package::Package(const istring& name, Package* parent)
:   m_refcount(0)
,   m_name(name)
,   m_parent(parent)
{
    static std::vector< refptr<Package> > s_orphans;
    refptr<Package> obj(this);
    std::pair< minitl::map<istring,refptr<Package> >::iterator, bool > result = parent->m_children.insert(std::make_pair(name, obj));
    if(!result.second)
    {
        AssertNotReached();
        s_orphans.push_back(obj);
    }
}

Package::~Package()
{
    Assert(m_refcount == 0);
}

refptr<Package> Package::MetaClass::create(const ipath& name) const
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

Package* Package::get(const inamespace& name)
{
    UNUSED(name);
/*  static Package s_root;
    Package* result = &s_root;
    for(size_t i = 0; i < name.size(); ++i)
    {
        stdhash::hash_map<istring,refptr<Package> >::iterator it = result->m_children.find(name[i]);
        if(it == result->m_children.end())
            return 0;
        result = *it->second;
    }
    return result;*/
    return 0;
}

}}
