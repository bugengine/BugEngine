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

namespace BugEngine { namespace Graphics { namespace OpenGL
{

VertexBuffer::VertexBuffer(const Renderer* /*renderer*/, u32 /*vertexCount*/, VertexUsage /*usage*/, VertexBufferFlags flags)
{
    size_t i;
    for(i = 0; i < flags.elementCount; ++i)
    {
        switch(flags.info[i].type)
        {
        case VertexElementInfo::float1:
            break;
        case VertexElementInfo::float2:
            break;
        case VertexElementInfo::float3:
            break;
        case VertexElementInfo::float4:
            break;
        case VertexElementInfo::color32:
            break;
        case VertexElementInfo::byte4:
            break;
        case VertexElementInfo::short2:
            break;
        case VertexElementInfo::short4:
            break;
        case VertexElementInfo::ushort2:
            break;
        case VertexElementInfo::ushort4:
            break;
        default:
            AssertNotReached();
        };
        switch(flags.info[i].semantic)
        {
        case gPosition:
            break;
        case gNormal:
            break;
        case gDiffuse:
            break;
        case gSpecular:
            break;
        case gUV0:
            break;
        case gUV1:
            break;
        case gUV2:
            break;
        case gUV3:
            break;
        case gUV4:
            break;
        case gUV5:
            break;
        case gUV6:
            break;
        case gUV7:
            break;
        default:
            AssertNotReached();
        }
    }
}

VertexBuffer::~VertexBuffer()
{
}

void* VertexBuffer::map(GpuMapFlags /*flags*/, u32 /*byteCount*/, u32 /*byteOffset*/)
{
    return 0;
}

void VertexBuffer::unmap()
{
}


}}}
