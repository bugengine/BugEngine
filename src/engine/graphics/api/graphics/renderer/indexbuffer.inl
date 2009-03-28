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


#ifndef BE_GRAPHICS_INDEXBUFFER_INL_
#define BE_GRAPHICS_INDEXBUFFER_INL_
/*****************************************************************************/
#include    <graphics/renderer/renderbackend.hh>

namespace BugEngine { namespace Graphics
{

template< >
inline IndexBuffer<u16>::IndexBuffer(const RenderBackend* owner, size_t vertexCount, IndexUsage usage)
:   m_owner(owner)
,   m_buffer(m_owner->createIndexBuffer(vertexCount, usage, IndexBufferFlags(IndexBufferFlags::Short)))
{
}

template< >
inline IndexBuffer<u32>::IndexBuffer(const RenderBackend* owner, size_t vertexCount, IndexUsage usage)
:   m_owner(owner)
,   m_buffer(m_owner->createIndexBuffer(vertexCount, usage, IndexBufferFlags(IndexBufferFlags::Long)))
{
}

template< typename T >
IndexBuffer<T>::~IndexBuffer()
{
}

template< typename T >
T* IndexBuffer<T>::map(size_t count, size_t offset)
{
    return static_cast<T*>(m_buffer->map(GpuMapFlags(), count*sizeof(T), offset*sizeof(T)));
}

template< typename T >
void IndexBuffer<T>::unmap()
{
    m_buffer->unmap();
}

template< typename T >
const GpuBuffer* IndexBuffer<T>::buffer() const
{
    return m_buffer.get();
}


}}

#endif
