/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_INDEXBUFFER_INL_
#define BE_GRAPHICS_INDEXBUFFER_INL_
/*****************************************************************************/
#include    <graphics/renderer/irenderer.hh>

namespace BugEngine { namespace Graphics
{

template< >
inline IndexBuffer<u16>::IndexBuffer(weak<const IRenderer> owner, u32 vertexCount, IndexUsage usage)
:   m_owner(owner)
,   m_buffer(m_owner->createIndexBuffer(vertexCount, usage, IndexBufferFlags(IndexBufferFlags::Short)))
{
}

template< >
inline IndexBuffer<u32>::IndexBuffer(weak<const IRenderer> owner, u32 vertexCount, IndexUsage usage)
:   m_owner(owner)
,   m_buffer(m_owner->createIndexBuffer(vertexCount, usage, IndexBufferFlags(IndexBufferFlags::Long)))
{
}

template< typename T >
IndexBuffer<T>::~IndexBuffer()
{
}

template< typename T >
T* IndexBuffer<T>::map(u32 count, u32 offset)
{
    return static_cast<T*>(m_buffer->map(GpuMapFlags(), count*(u32)sizeof(T), offset*(u32)sizeof(T)));
}

template< typename T >
void IndexBuffer<T>::unmap()
{
    m_buffer->unmap();
}

template< typename T >
weak<const GpuBuffer> IndexBuffer<T>::buffer() const
{
    return m_buffer;
}


}}

#endif
