/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/tools/ishaderbuilder.hh>

namespace BugEngine { namespace Graphics { namespace Shaders
{

IShaderBuilder::Namespace::Namespace()
    :   names(tempArena())
{
};

IShaderBuilder::IShaderBuilder()
    :   m_stream(tempArena(), 10000)
    ,   m_namespaces(tempArena())
    ,   m_input()
    ,   m_output()
    ,   m_indent(0)
    ,   m_counter(0)
{
    m_stream.write("\0", 1);
    m_stream.seek(MemoryStream::eSeekMove, -1);
    m_namespaces.push_back(Namespace());
}

IShaderBuilder::~IShaderBuilder()
{
}

void IShaderBuilder::indent()
{
    m_indent++;
}

void IShaderBuilder::unindent()
{
    m_indent--;
}

void IShaderBuilder::write(const char *text)
{
    size_t size = text ? strlen(text) : 0;
    if (size)
    {
        for(int i = 0; i < m_indent; ++i)
            m_stream.write("  ", 2);
        m_stream.write(text, strlen(text));
    }
    m_stream.write("\n", 1);
    m_stream.write("\0", 1);
    m_stream.seek(MemoryStream::eSeekMove, -1);
}

const char *IShaderBuilder::text() const
{
    return reinterpret_cast<const char *>(m_stream.basememory());
}

i64 IShaderBuilder::textSize() const
{
    return m_stream.offset();
}

istring IShaderBuilder::referenceNode(weak<const Node> node)
{
    for(minitl::vector< Namespace >::const_reverse_iterator it = m_namespaces.rbegin(); it != m_namespaces.rend(); ++it)
    {
        minitl::hashmap< weak<const Node>, istring >::const_iterator name = it->names.find(node);
        if (name != it->names.end())
        {
            return(name->second.c_str());
        }
    }
    be_error("Undeclared object");
    return istring("");
}

void IShaderBuilder::addUniform(weak<const Node> node, Stage stage, const istring& name, Type type)
{
    bool inserted = m_namespaces.front().names.insert(std::make_pair(node, name)).second;
    if (inserted)
    {
        doAddUniformDeclaration(name, stage, type);
    }
}

void IShaderBuilder::addVarying(weak<const Node> node, Stage stage, const istring& name, Type type)
{
    bool inserted = m_namespaces.front().names.insert(std::make_pair(node, name)).second;
    if (inserted)
    {
        doAddVaryingDeclaration(name, stage, type);
    }
}

}}}
