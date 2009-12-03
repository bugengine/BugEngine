/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <data/stdafx.h>
#include    <parse/database.hh>
#include    <parse/node.hh>
#include    <parse/context.hh>

#include    <core/memory/streams.hh>
#include    <system/filesystem.hh>

namespace BugEngine { namespace Data { namespace Parse
{

Database::DatabaseElement::DatabaseElement(const istring& name, weak<DatabaseElement> parent) :
    m_name(name),
    m_parent(parent)
{
}

Database::DatabaseElement::~DatabaseElement()
{
}

void Database::DatabaseElement::add(const istring &name, Visibility v, ref<const Node> value)
{
    std::pair< ChildrenContainer::iterator, bool> insertResult = m_objects.insert(std::make_pair(name, std::make_pair(v, value)));
    be_assert(insertResult.second, "database element %s already exist" | name.c_str());
    insertResult.first->second.first = v;
    insertResult.first->second.second = value;
}

weak<Database::DatabaseElement> Database::DatabaseElement::push(const istring &name)
{
    std::pair< NamespaceContainer::iterator, bool> insertResult = m_namespaces.insert(std::make_pair(name, ref<DatabaseElement>()));
    if(insertResult.second)
    {
        insertResult.first->second = ref<DatabaseElement>::create(name, this);
    }
    return insertResult.first->second;
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
    ref<RTTI::Namespace> ns = ref<RTTI::Namespace>::create();
    for(ChildrenContainer::const_iterator it = m_objects.begin(); it != m_objects.end(); ++it)
        if(it->second.first == Export)
            ns->set(it->first, it->second.second->eval(context));
    for(NamespaceContainer::const_iterator it = m_namespaces.begin(); it != m_namespaces.end(); ++it)
    {
        Value v = it->second->eval(context);
        be_assert(v.type() == RTTI::PropertyTypeObject, "invalid type");
        ref<RTTI::Namespace> ns = v.as< ref<RTTI::Namespace> >();
        ns->mount(it->first, ns);
    }

    return Value(ref<Object>(ns));
}

//-----------------------------------------------------------------------------

extern weak<AbstractMemoryStream> g_parseStream;

Database::Database()
:   m_root(ref<DatabaseElement>::create("", weak<DatabaseElement>()))
,   m_current(m_root)
{
}

Database::~Database()
{
}

void Database::add(const istring& name, Visibility v, ref<const Node> value)
{
    m_current->add(name, v, value);
}

void Database::push(const istring& name)
{
    m_current = m_current->push(name);
}

void Database::pop(const istring& name)
{
    be_assert(m_current->m_name == name, "asymetric push/pop; should pop %s, popping %s" | m_current->m_name.c_str() | name.c_str());
    m_current = m_current->m_parent;
}

void Database::parse(const ifilename& file)
{
    ParseParam params(file, this);

    ref<AbstractMemoryStream> stream = FileSystem::instance()->open(file, eReadOnly);
    g_parseStream = stream;

    yyparse(&params);

    g_parseStream = 0;
}

ref<RTTI::Namespace> Database::commit()
{
    Context ctx(*this);
    m_root->dolink(ctx);
    Value value = m_root->doeval(ctx);
    be_assert(value.type() == RTTI::PropertyTypeObject, "invalid type");
    return value.as< ref<RTTI::Namespace> >();
}


}}}
