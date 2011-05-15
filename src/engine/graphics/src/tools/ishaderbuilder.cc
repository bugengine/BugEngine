/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/tools/ishaderbuilder.hh>

namespace BugEngine { namespace Graphics
{

IShaderBuilder::IShaderBuilder()
    :   m_stream(tempArena(), 10000)
    ,   m_indent(0)
{
    m_stream.write("\0", 1);
    m_stream.seek(MemoryStream::eSeekMove, -1);
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

}}
