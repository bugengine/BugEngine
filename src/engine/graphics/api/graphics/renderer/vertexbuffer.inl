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


#ifndef BE_GRAPHICS_VERTEXBUFFER_INL_
#define BE_GRAPHICS_VERTEXBUFFER_INL_
/*****************************************************************************/
#include    <graphics/renderer/renderer.hh>

namespace BugEngine { namespace Graphics
{

template< typename T >
VertexBuffer<T>::VertexBuffer(const RenderBackend* owner, u32 vertexCount, VertexUsage usage)
:   m_owner(owner)
,   m_buffer(m_owner->createVertexBuffer(vertexCount, usage, T::getFlags()))
{
    be_assert(sizeof(T) == T::getFlags().vertexSize, "sizes don't match for vertex buffer; vertex type is %s, type size is %d and storage size is %d" | typeid(T).name() | sizeof(T) | T::getFlags().vertexSize);
}

template< typename T >
VertexBuffer<T>::~VertexBuffer()
{
}

template< typename T >
T* VertexBuffer<T>::map(u32 count, u32 offset)
{
    return static_cast<T*>(m_buffer->map(GpuMapFlags(), count*(u32)sizeof(T), offset*(u32)sizeof(T)));
}

template< typename T >
void VertexBuffer<T>::unmap()
{
    m_buffer->unmap();
}

template< typename T >
const GpuBuffer* VertexBuffer<T>::buffer() const
{
    return m_buffer.get();
}

}}

#endif
