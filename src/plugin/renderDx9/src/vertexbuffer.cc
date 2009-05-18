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

#include    <stdafx.h>
#include    <vertexbuffer.hh>

namespace BugEngine { namespace Graphics { namespace DirectX9
{

VertexBuffer::VertexBuffer(const Renderer* renderer, size_t vertexCount, VertexUsage usage, VertexBufferFlags flags)
{
    DWORD fvf = 0;
    size_t i;
    for(i = 0; i < flags.elementCount; ++i)
    {
        m_vertexElements[i].Stream = 0;
        m_vertexElements[i].Offset = checked_numcast<WORD>(flags.info[i].offset);
        switch(flags.info[i].type)
        {
        case VertexElementInfo::float1:
            m_vertexElements[i].Type = D3DDECLTYPE_FLOAT1;
            break;
        case VertexElementInfo::float2:
            m_vertexElements[i].Type = D3DDECLTYPE_FLOAT2;
            break;
        case VertexElementInfo::float3:
            m_vertexElements[i].Type = D3DDECLTYPE_FLOAT3;
            break;
        case VertexElementInfo::float4:
            m_vertexElements[i].Type = D3DDECLTYPE_FLOAT4;
            break;
        case VertexElementInfo::color32:
            m_vertexElements[i].Type = D3DDECLTYPE_UBYTE4N;
            break;
        case VertexElementInfo::byte4:
            m_vertexElements[i].Type = D3DDECLTYPE_UBYTE4N;
            break;
        case VertexElementInfo::short2:
            m_vertexElements[i].Type = D3DDECLTYPE_SHORT2N;
            break;
        case VertexElementInfo::short4:
            m_vertexElements[i].Type = D3DDECLTYPE_SHORT4N;
            break;
        case VertexElementInfo::ushort2:
            m_vertexElements[i].Type = D3DDECLTYPE_USHORT2N;
            break;
        case VertexElementInfo::ushort4:
            m_vertexElements[i].Type = D3DDECLTYPE_USHORT4N;
            break;
        default:
            AssertNotReached();
        };
        m_vertexElements[i].Method = D3DDECLMETHOD_DEFAULT;
        switch(flags.info[i].semantic)
        {
        case gPosition:
            m_vertexElements[i].Usage = D3DDECLUSAGE_POSITION;
            m_vertexElements[i].UsageIndex = 0;
            fvf |= D3DFVF_XYZ;
            break;
        case gNormal:
            m_vertexElements[i].Usage = D3DDECLUSAGE_NORMAL;
            m_vertexElements[i].UsageIndex = 0;
            fvf |= D3DFVF_NORMAL;
            break;
        case gDiffuse:
            m_vertexElements[i].Usage = D3DDECLUSAGE_COLOR;
            m_vertexElements[i].UsageIndex = 0;
            fvf |= D3DFVF_DIFFUSE;
            break;
        case gSpecular:
            m_vertexElements[i].Usage = D3DDECLUSAGE_COLOR;
            m_vertexElements[i].UsageIndex = 1;
            fvf |= D3DFVF_SPECULAR;
            break;
        case gUV0:
            m_vertexElements[i].Usage = D3DDECLUSAGE_TEXCOORD;
            m_vertexElements[i].UsageIndex = 0;
            fvf |= D3DFVF_TEX0;
            break;
        case gUV1:
            m_vertexElements[i].Usage = D3DDECLUSAGE_TEXCOORD;
            m_vertexElements[i].UsageIndex = 1;
            break;
        case gUV2:
            m_vertexElements[i].Usage = D3DDECLUSAGE_TEXCOORD;
            m_vertexElements[i].UsageIndex = 2;
            break;
        case gUV3:
            m_vertexElements[i].Usage = D3DDECLUSAGE_TEXCOORD;
            m_vertexElements[i].UsageIndex = 3;
            break;
        case gUV4:
            m_vertexElements[i].Usage = D3DDECLUSAGE_TEXCOORD;
            m_vertexElements[i].UsageIndex = 4;
            break;
        case gUV5:
            m_vertexElements[i].Usage = D3DDECLUSAGE_TEXCOORD;
            m_vertexElements[i].UsageIndex = 5;
            break;
        case gUV6:
            m_vertexElements[i].Usage = D3DDECLUSAGE_TEXCOORD;
            m_vertexElements[i].UsageIndex = 6;
            break;
        case gUV7:
            m_vertexElements[i].Usage = D3DDECLUSAGE_TEXCOORD;
            m_vertexElements[i].UsageIndex = 7;
            break;
        default:
            AssertNotReached();
        }
    }
    D3DVERTEXELEMENT9 end = D3DDECL_END();
    m_vertexElements[i] = end;
    fvf = 0;
    renderer->m_device->CreateVertexBuffer(vertexCount*flags.vertexSize, (usage == VuStatic?0:D3DUSAGE_DYNAMIC) | D3DUSAGE_WRITEONLY, fvf, D3DPOOL_DEFAULT, &m_buffer, 0);
    D3D_CHECKRESULT(renderer->m_device->CreateVertexDeclaration(m_vertexElements, &m_vertexDecl));
    m_vertexStride = flags.vertexSize;
}

VertexBuffer::~VertexBuffer()
{
    m_buffer->Release();
}

void* VertexBuffer::map(GpuMapFlags flags, size_t byteCount, size_t byteOffset)
{
    void* data;
    D3D_CHECKRESULT(m_buffer->Lock(byteOffset, byteCount, &data, D3DLOCK_DISCARD));
    return data;
}

void VertexBuffer::unmap()
{
    D3D_CHECKRESULT(m_buffer->Unlock());
}


}}}
