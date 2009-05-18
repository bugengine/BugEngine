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
#include    <parse/database.hh>
#include    <parse/node.hh>
#include    <parse/context.hh>

#include    <core/memory/streams.hh>
#include    <system/filesystem.hh>

namespace BugEngine { namespace Data { namespace Parse
{

Database::DatabaseElement::DatabaseElement(const istring& name, DatabaseElement* parent) :
    m_name(name),
    m_parent(parent)
{
}

Database::DatabaseElement::~DatabaseElement()
{
}

void Database::DatabaseElement::add(const istring &name, Visibility v, refptr<const Node> value)
{
    std::pair< ChildrenContainer::iterator, bool> insertResult = m_objects.insert(std::make_pair(name, std::make_pair(v, value)));
    Assert(insertResult.second);
    insertResult.first->second.first = v;
    insertResult.first->second.second = value;
}

Database::DatabaseElement* Database::DatabaseElement::push(const istring &name)
{
    std::pair< NamespaceContainer::iterator, bool> insertResult = m_namespaces.insert(std::make_pair(name, refptr<DatabaseElement>(0)));
    if(insertResult.second)
    {
        insertResult.first->second = new DatabaseElement(name, this);
    }
    return insertResult.first->second.get();
}

void Database::DatabaseElement::dolink(Context& context) const
{
    for(ChildrenContainer::const_iterator it = m_objects.begin(); it != m_objects.end(); ++it)
        it->second.second->link(context);
    for(NamespaceContainer::const_iterator it = m_namespaces.begin(); it != m_namespaces.end(); ++it)
        it->second->link(context);
}

Value Database::DatabaseElement::doeval(Context& context) const
{
    Namespace* ns = new Namespace();
    for(ChildrenContainer::const_iterator it = m_objects.begin(); it != m_objects.end(); ++it)
        if(it->second.first == Export)
            ns->set(it->first, it->second.second->eval(context));
    for(NamespaceContainer::const_iterator it = m_namespaces.begin(); it != m_namespaces.end(); ++it)
    {
        Value v = it->second->eval(context);
        Assert(v.type() == RTTI::PropertyTypeObject);
        Namespace* ns = v.as< Namespace* >();
        ns->mount(it->first, ns);
    }

    return Value(refptr<Object>(ns));
}

//-----------------------------------------------------------------------------

extern AbstractMemoryStream* g_parseStream;

Database::Database() :
    m_root(new DatabaseElement("", 0)),
    m_current(m_root.get())
{
}

Database::~Database()
{
}

void Database::add(const istring& name, Visibility v, refptr<const Node> value)
{
    m_current->add(name, v, value);
}

void Database::push(const istring& name)
{
    m_current = m_current->push(name);
}

void Database::pop(const istring& name)
{
    Assert(m_current->m_name == name);
    m_current = m_current->m_parent;
}

void Database::parse(const ifilename& file)
{
    ParseParam params(file, this);

    refptr<AbstractMemoryStream> stream = FileSystem::instance()->open(file, eReadOnly);
    g_parseStream = stream.get();

    yyparse(&params);

    g_parseStream = 0;
}

refptr<Namespace> Database::commit()
{
    Context ctx(*this);
    m_root->dolink(ctx);
    Value value = m_root->doeval(ctx);
    Assert(value.type() == RTTI::PropertyTypeObject);
    return value.as< refptr<Namespace> >();
}


}}}
