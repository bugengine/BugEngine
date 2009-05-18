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

#include    <data/stdafx.h>
#include    <data/package.hh>

#include    <parse/database.hh>
#include    <system/filesystem.hh>

namespace BugEngine
{

METACLASS_IMPL("",Package);

static Package* getParent(ipath name)
{
    name.pop_back();
    //return Package::get(name);
    return 0;
}

Package::Package(const ipath& name)
:   RTTI::Package(name[name.size()-1], 0)
,   m_namespace(0)
{
}

Package::~Package()
{
    Assert(m_refcount == 0);
}

refptr<RTTI::Package> Package::MetaClass::create(const ipath& name) const
{
    return new Package(name);
}

void Package::doload()
{
    Data::Parse::Database db;
/*    std::set<ipath> dirs = BugEngine::FileSystem::instance()->listDirectories("data/scripts" + name());
    std::set<ifilename> files = BugEngine::FileSystem::instance()->listFiles("data/scripts" + name(), ".dd");
    for(std::set<ifilename>::const_iterator it = files.begin(); it != files.end(); ++it)
    {
        db.parse(*it);
    }
    m_namespace = db.commit();*/
}

void Package::dounload()
{
    m_namespace = 0;
}

const Namespace* Package::getNamespace() const
{
    return m_namespace.get();
}

}
