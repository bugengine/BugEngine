/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_RENDERER_BUFFER_VERTEXBUFFER_INL_
#define BE_GRAPHICS_RENDERER_BUFFER_VERTEXBUFFER_INL_
/*****************************************************************************/

namespace BugEngine { namespace Graphics
{

template< typename T >
VertexBuffer<T>::VertexBuffer(u32 vertexCount, VertexUsage usage)
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
weak<const GpuBuffer> VertexBuffer<T>::buffer() const
{
    return m_buffer;
}

}}

#endif
