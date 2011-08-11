/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <3d/stdafx.h>
#include    <3d/shader/ishaderbuilder.hh>

namespace BugEngine { namespace Graphics { namespace Shaders
{

IShaderBuilder::Namespace::Namespace()
    :   names(tempArena())
{
};

IShaderBuilder::IShaderBuilder()
    :   m_namespaces(tempArena())
    ,   m_currentAttribute(0)
    ,   m_currentVarying(0)
    ,   m_currentAttributeToVarying(0)
    ,   m_currentTemporary(0)
    ,   m_attributeToVarying(tempArena())
    ,   m_stream(tempArena(), 10000)
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
    m_stream.write("\0", 1);
    m_stream.seek(MemoryStream::eSeekMove, -1);
}

void IShaderBuilder::writeln(const char *text)
{
    write(text);
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
    if (m_namespaces.front().names.insert(std::make_pair(node, name)).second)
    {
        doAddUniformDeclaration(name, stage, type);
    }
}

void IShaderBuilder::addVarying(weak<const Node> node, Stage currentStage, Stage targetStage, Type type)
{
    if (currentStage == targetStage)
    {
        istring name = minitl::format<>("b_varying%d") | m_currentVarying;
        if (m_namespaces.front().names.insert(std::make_pair(node, name)).second)
        {
            m_currentVarying++;
            doAddVaryingDeclaration(name, currentStage, type);
        }
    }
}

void IShaderBuilder::addAttribute(weak<const Node> node, Stage currentStage, Stage targetStage, Type type)
{
    if (VertexStage == targetStage)
    {
        istring nameAttribute = minitl::format<>("b_attribute%d") | m_currentAttribute;
        istring nameVarying = minitl::format<>("b_attributeToVarying%d") | m_currentAttributeToVarying;
        if (m_namespaces.front().names.insert(std::make_pair(node, nameAttribute)).second)
        {
            m_currentAttribute++;
            if (currentStage == VertexStage)
            {
                doAddAttributeDeclaration(nameAttribute, targetStage, type);
            }
            else
            {
                m_currentAttributeToVarying++;
                doAddAttributeDeclaration(nameAttribute, targetStage, type);
                doAddVaryingDeclaration(nameVarying, targetStage, type);
                m_attributeToVarying.push_back(minitl::make_pair(nameAttribute, nameVarying));
            }
        }
    }
    else if(currentStage == targetStage)
    {
        istring nameVarying = minitl::format<>("b_attributeToVarying%d") | m_currentAttributeToVarying;
        if (m_namespaces.front().names.insert(std::make_pair(node, nameVarying)).second)
        {
            m_currentAttributeToVarying++;
            doAddVaryingDeclaration(nameVarying, targetStage, type);
        }
    }
}

void IShaderBuilder::forwardAttributes()
{
    for (minitl::vector< minitl::pair<istring, istring> >::const_iterator it = m_attributeToVarying.begin(); it != m_attributeToVarying.end(); ++it)
    {
        doSaveTo(it->second, it->first);
    }
}

void IShaderBuilder::beginMethodDefinition(const istring& name)
{
    m_namespaces.push_back(Namespace());
    doAddMethod(name);
}

void IShaderBuilder::end()
{
    m_namespaces.erase(m_namespaces.end()-1);
    doEndMethod();
}

void IShaderBuilder::saveTo(Semantic semantic, weak<const Node> node)
{
    doSaveTo(semantic, referenceNode(node));
}

void IShaderBuilder::addOperator(weak<const Node> node, Operator op, Type type, weak<const Node> node1, weak<const Node> node2)
{
    istring var = minitl::format<>("temp_%d") | m_currentTemporary;
    if (m_namespaces.front().names.insert(std::make_pair(node, var)).second)
    {
        m_currentTemporary++;
        const istring& op1 = referenceNode(node1);
        const istring& op2 = referenceNode(node2);
        doAddOperator(op, type, var, op1, op2);
    }
}

}}}
