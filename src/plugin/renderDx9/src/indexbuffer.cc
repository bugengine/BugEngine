/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

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
