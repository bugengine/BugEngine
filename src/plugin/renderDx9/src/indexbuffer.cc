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
#include    <indexbuffer.hh>

namespace BugEngine { namespace Graphics { namespace DirectX9
{

IndexBuffer::IndexBuffer(const Renderer* renderer, u32 vertexCount, IndexUsage usage, IndexBufferFlags flags)
{
    renderer->m_device->CreateIndexBuffer(vertexCount*(flags.format == flags.Long?4:2), (usage == IuStatic?0:D3DUSAGE_DYNAMIC) | D3DUSAGE_WRITEONLY, (flags.format == flags.Long?D3DFMT_INDEX32:D3DFMT_INDEX16), D3DPOOL_DEFAULT, &m_buffer, 0);
}

IndexBuffer::~IndexBuffer()
{
    m_buffer->Release();
}

void* IndexBuffer::map(GpuMapFlags /*flags*/, u32 byteCount, u32 byteOffset)
{
    void* data;
    D3D_CHECKRESULT(m_buffer->Lock(byteOffset, byteCount, &data, D3DLOCK_DISCARD));
    return data;
}

void IndexBuffer::unmap()
{
    D3D_CHECKRESULT(m_buffer->Unlock());
}


}}}
