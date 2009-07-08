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
#include    <graphics/primitive/cube.hh>

namespace BugEngine { namespace Graphics
{

be_metaclass_impl("Graphics",Cube)

Cube::Cube(const RenderBackend* renderer)
:   Primitive(renderer)
,   m_vertexBuffer(new VertexBuffer<VertexFormat>(renderer, 8, VuStatic))
,   m_indexBuffer(new IndexBuffer<u16>(renderer, 36, IuStatic))
{
    VertexFormat* v = m_vertexBuffer->map();
    v[0].position = float4(0.0f, 0.0f, 0.0f, 1.0f);
    v[1].position = float4(1.0f, 0.0f, 0.0f, 1.0f);
    v[2].position = float4(0.0f, 1.0f, 0.0f, 1.0f);
    v[3].position = float4(1.0f, 1.0f, 0.0f, 1.0f);
    v[4].position = float4(0.0f, 0.0f, 1.0f, 1.0f);
    v[5].position = float4(1.0f, 0.0f, 1.0f, 1.0f);
    v[6].position = float4(0.0f, 1.0f, 1.0f, 1.0f);
    v[7].position = float4(1.0f, 1.0f, 1.0f, 1.0f);
    m_vertexBuffer->unmap();

    u16* i = m_indexBuffer->map();
    i[0] = 0;
    i[1] = 1;
    i[2] = 2;
    i[3] = 1;
    i[4] = 3;
    i[5] = 2;

    i[6] = 4;
    i[7] = 0;
    i[8] = 2;
    i[9] = 4;
    i[10] = 2;
    i[11] = 6;

    i[12] = 1;
    i[13] = 5;
    i[14] = 3;
    i[15] = 3;
    i[16] = 5;
    i[17] = 7;

    i[18] = 5;
    i[19] = 4;
    i[20] = 6;
    i[21] = 5;
    i[22] = 6;
    i[23] = 7;

    i[24] = 2;
    i[25] = 3;
    i[26] = 6;
    i[27] = 3;
    i[28] = 7;
    i[29] = 6;

    i[30] = 1;
    i[31] = 0;
    i[32] = 4;
    i[33] = 1;
    i[34] = 4;
    i[35] = 5;
    m_indexBuffer->unmap();
}

Cube::~Cube()
{
}

const GpuBuffer* Cube::indices() const
{
    return m_indexBuffer->buffer();
}

const GpuBuffer* Cube::vertices() const
{
    return m_vertexBuffer->buffer();
}

unsigned Cube::nbVertices() const
{
    return 36;
}

}}
