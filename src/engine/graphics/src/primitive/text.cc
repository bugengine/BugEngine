/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/primitive/text.hh>

namespace BugEngine { namespace Graphics
{

Text::Text(weak<const RenderBackend> renderer, ref<const Font> font, const char * /*message*/)
:   Primitive(renderer)
,   m_font(font)
,   m_indexBuffer()
,   m_totalVertices(0)
{
}

Text::~Text()
{
}

weak<const GpuBuffer> Text::indices() const
{
    return m_indexBuffer->buffer();
}

weak<const GpuBuffer> Text::vertices() const
{
    return m_font->vertices();
}

unsigned Text::nbVertices() const
{
    return m_totalVertices;
}

}}
