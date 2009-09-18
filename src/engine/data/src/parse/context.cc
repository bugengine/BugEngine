/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <data/stdafx.h>
#include    <parse/context.hh>
#include    <parse/database.hh>

namespace BugEngine { namespace Data { namespace Parse
{

Context::Context(Database& db) :
    m_database(db)
{
}

Context::~Context()
{
}

Database& Context::database()
{
    return m_database;
}

const Database& Context::database() const
{
    return m_database;
}

}}}
