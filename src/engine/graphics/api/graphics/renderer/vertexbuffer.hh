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

#ifndef BE_GRAPHICS_VERTEXBUFFER_HH_
#define BE_GRAPHICS_VERTEXBUFFER_HH_
/*****************************************************************************/
#include    <graphics/renderer/gpubuffer.hh>
#include    <graphics/renderer/vertexdesc.hh>

namespace BugEngine { namespace Graphics
{

class RenderBackend;

template< typename T >
class VertexBuffer : public minitl::refcountable<void>
{
private:
    const RenderBackend*    m_owner;
    refptr<GpuBuffer>       m_buffer;
public:
    VertexBuffer(const RenderBackend* owner, size_t vertexCount, VertexUsage usage);
    ~VertexBuffer();

    T*  map(size_t count = 0, size_t offset = 0);
    void unmap();

    const GpuBuffer* buffer() const;
};

}}

#include    <graphics/renderer/vertexbuffer.inl>

/*****************************************************************************/
#endif
