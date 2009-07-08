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

#include    <graphics/stdafx.h>
#include    <graphics/primitive/text.hh>

namespace BugEngine { namespace Graphics
{

be_abstractmetaclass_impl("Graphics",Text)

Text::Text(const RenderBackend* renderer, const Font* font, const char * /*message*/)
:   Primitive(renderer)
,   m_font(font)
,   m_indexBuffer(0)
,   m_totalVertices(0)
{
}

Text::~Text()
{
}

const GpuBuffer* Text::indices() const
{
    return m_indexBuffer->buffer();
}

const GpuBuffer* Text::vertices() const
{
    return m_font->vertices();
}

unsigned Text::nbVertices() const
{
    return m_totalVertices;
}

}}
