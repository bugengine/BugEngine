/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <data/stdafx.h>
#include    <data/package.hh>

#include    <parse/database.hh>
#include    <system/filesystem.hh>

namespace BugEngine { namespace Data
{

be_abstractmetaclass_impl("",Package);

Package::Package(const ipath& name)
:   RTTI::Package(name[name.size()-1], 0)
,   m_namespace()
{
}

Package::~Package()
{
    be_assert(m_refcount == 0, "destroying package that is still in use");
}

ref<RTTI::Package> Package::MetaClass::create(const ipath& name) const
{
    return ref<Package>::create(name);
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
    m_namespace.clear();
}

weak<const RTTI::Namespace> Package::getNamespace() const
{
    return m_namespace;
}

}}
